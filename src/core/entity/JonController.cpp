//
//  JonController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI(JonController, EntityController)
IMPL_EntityController_create(JonController)

JonController::JonController(Entity* e) : EntityController(e),
_shockwaveStartTime(0),
_shockwaveStateTime(0),
_isReleasingShockwave(false)
{
    // Empty
}

void JonController::processInput(uint16_t inputState)
{
    if (isMovementInputAllowed())
    {
        processMovementInput(inputState);
    }
    
    if (isAttackInputAllowed())
    {
        processAttackInput(inputState);
    }
    
    if (isAbilityInputAllowed())
    {
        processAbilityInput(inputState);
    }
    
    if (isWarmingUpInputAllowed())
    {
        processWarmingUpInput(inputState);
    }
    
    if (isSpecialInputAllowed())
    {
        processSpecialInput(inputState);
    }
    
    if (isJumpInputAllowed())
    {
        processJumpInput(inputState);
    }
}

void JonController::onUpdate(uint32_t numMovesProcessed)
{
    Entity& e = *_entity;
    Vector2& vel = e.velocity();
    uint8_t& state = e.state()._state;
    uint8_t& stateFlags = e.state()._stateFlags;
    uint16_t& stateTime = e.state()._stateTime;
    
    std::string textureMapping = e.renderController()->getTextureMapping();
    Animation* animation = ASSETS_MGR.animation(textureMapping);
    uint16_t animationNumFrames = animation == nullptr ? 1 : animation->cycleTime();
    uint16_t animationNumFramesBeforeFirstLoopingFrame = animation == nullptr ? 1 : animation->cycleTimeBeforeFirstLoopingFrame();
    
    _shockwaveStateTime = numMovesProcessed - _shockwaveStartTime;
    if (_isReleasingShockwave &&
        _shockwaveStateTime > 120)
    {
        _isReleasingShockwave = false;
    }
    
    // Rabbit
    if (state == STATE_DRILL_STOMPING)
    {
        if (e.isGrounded())
        {
            static float speedX = e.data().getFloat("speedX");
            
            state = STATE_LANDING_ROLL;
            stateTime = 0;
            float landingRollSpeedX = e.pose()._isXFlipped ? -speedX : speedX;
            vel._x = landingRollSpeedX * 2;
        }
        else if (stateTime == animationNumFramesBeforeFirstLoopingFrame)
        {
            vel._y = e.data().getFloat("gravity") * 2;
            e.pose()._isZeroGravity = false;
        }
    }
    
    if (state == STATE_ATTEMPTING_TO_TRANSFORM &&
        stateTime == animationNumFrames)
    {
        state = STATE_TRANSFORMING;
        stateTime = 0;
        // TODO, figure out some way to get rid of local data in the Controller
        // Anything not networked is risky.
        _isReleasingShockwave = true;
        _shockwaveStartTime = numMovesProcessed;
        e.pose()._isSlowed = false;
        e.pose()._isZeroGravity = false;
    }
    
    // Vampire
    if (state == STATE_VAMPIRE_DOUBLE_JUMPING &&
        stateTime == animationNumFrames)
    {
        state = STATE_VAMPIRE_GLIDING;
        stateTime = 0;
    }
    
    if (state == STATE_VAMPIRE_DASHING)
    {
        if (stateTime == animationNumFrames)
        {
            state = STATE_VAMPIRE_DASH_COMPLETED;
            stateTime = 0;
            vel._x = 0;
            e.pose()._isZeroGravity = false;
        }
    }
    
    if (state == STATE_ATTEMPTING_TO_REVERT &&
        stateTime == animationNumFrames)
    {
        state = STATE_REVERTING;
        stateTime = 0;
        _isReleasingShockwave = true;
        _shockwaveStartTime = numMovesProcessed;
        e.pose()._isSlowed = false;
        e.pose()._isZeroGravity = false;
    }
    
    // Both Rabbit and Vampire
    if (
        canCurrentStateBeInterruptedByFalling() &&
        !e.isGrounded() &&
        vel._y < 0)
    {
        State doubleJumpingState = isRabbit() ? STATE_DOUBLE_JUMPING : STATE_VAMPIRE_DOUBLE_JUMPING;
        State fallingState = isRabbit() ? STATE_FALLING : STATE_VAMPIRE_FALLING;
        
        bool canJumpAgain = true;
        if (state == doubleJumpingState)
        {
            canJumpAgain = false;
        }
        state = fallingState;
        SET_BIT(stateFlags, STATE_FALLING_FLAG_CAN_JUMP_AGAIN, canJumpAgain);
        stateTime = 0;
    }
    
    if (canCurrentStateBeInterruptedByLanding() &&
        e.isGrounded())
    {
        State landingState = isRabbit() ? STATE_LANDING : STATE_VAMPIRE_LANDING;
        
        state = landingState;
        SET_BIT(stateFlags, STATE_FALLING_FLAG_CAN_JUMP_AGAIN, true);
        stateTime = 0;
    }
    
    if (stateTime >= animationNumFrames &&
        needsToResolveNewStateOnceAnimationEnds())
    {
        resolveNewState();
    }
}

void JonController::onMessage(uint16_t message, Entity* fromEntity)
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    uint16_t& stateTime = e.state()._stateTime;
    Vector2& vel = e.velocity();
    
    State deathState = isRabbit() ? STATE_RABBIT_DEATH : STATE_VAMPIRE_DEATH;
    
    if (message == MSG_DAMAGE &&
        state != deathState)
    {
        state = deathState;
        stateTime = 0;
        vel.reset();
    }
}

void JonController::processMovementInput(uint16_t inputState)
{
    Entity& e = *_entity;
    
    bool isInputLeft = IS_BIT_SET(inputState, JISF_MOVING_LEFT);
    bool wasInputLeft = IS_BIT_SET(e.lastProcessedInputState(), JISF_MOVING_LEFT);
    bool isInputRight = IS_BIT_SET(inputState, JISF_MOVING_RIGHT);
    bool wasInputRight = IS_BIT_SET(e.lastProcessedInputState(), JISF_MOVING_RIGHT);
    
    static float speedRabbitX = e.data().getFloat("speedX");
    static float speedVampireX = e.data().getFloat("speedVampireX");
    float speedX = isRabbit() ? speedRabbitX : speedVampireX;
    
    uint8_t& state = e.state()._state;
    uint16_t& stateTime = e.state()._stateTime;
    Vector2& vel = e.velocity();
    bool isGrounded = e.isGrounded();
    
    State idleState = isRabbit() ? STATE_IDLE : STATE_VAMPIRE_IDLE;
    State runningState = isRabbit() ? STATE_RUNNING : STATE_VAMPIRE_RUNNING;
    
    if (isInputLeft)
    {
        if (needsStateChangeForMovement())
        {
            state = runningState;
            stateTime = 0;
        }
        vel._x = -speedX;
        e.pose()._isXFlipped = true;
    }
    else if (isInputRight)
    {
        if (needsStateChangeForMovement())
        {
            state = runningState;
            stateTime = 0;
        }
        vel._x = speedX;
        e.pose()._isXFlipped = false;
    }
    else if (wasInputLeft || wasInputRight)
    {
        if (isGrounded)
        {
            state = idleState;
            stateTime = 0;
        }
        vel._x = 0;
    }
}

void JonController::processJumpInput(uint16_t inputState)
{
    Entity& e = *_entity;
    
    bool isInputJump = IS_BIT_SET(inputState, JISF_JUMPING);
    bool wasInputJump = IS_BIT_SET(e.lastProcessedInputState(), JISF_JUMPING);
    
    static uint32_t numJumpFrames = e.data().getUInt("numJumpFrames");
    static uint32_t numDoubleJumpFrames = e.data().getUInt("numDoubleJumpFrames");
    static float speedRabbitY = e.data().getFloat("speedY");
    static float speedVampireY = e.data().getFloat("speedVampireY");
    float speedY = isRabbit() ? speedRabbitY : speedVampireY;
    
    uint8_t& state = e.state()._state;
    uint8_t& stateFlags = e.state()._stateFlags;
    uint16_t& stateTime = e.state()._stateTime;
    Vector2& vel = e.velocity();
    bool isGrounded = e.isGrounded();
    
    State jumpingState = isRabbit() ? STATE_JUMPING : STATE_VAMPIRE_JUMPING;
    State doubleJumpingState = isRabbit() ? STATE_DOUBLE_JUMPING : STATE_VAMPIRE_DOUBLE_JUMPING;
    State fallingState = isRabbit() ? STATE_FALLING : STATE_VAMPIRE_FALLING;
    
    if (state == STATE_VAMPIRE_GLIDING)
    {
        if (!isInputJump)
        {
            state = STATE_VAMPIRE_FALLING;
            stateTime = 0;
        }
        else if (isInputJump &&
            wasInputJump)
        {
            // Glide is really more like implementing reduced gravity
            vel._y = -16;
        }
    }
    else if (state == doubleJumpingState)
    {
        if (isInputJump &&
            wasInputJump &&
            stateTime < numDoubleJumpFrames)
        {
            // TODO, this allows the player to hold, let go, and hold again
            vel._y = speedY;
        }
    }
    else if (state == jumpingState)
    {
        if (isInputJump)
        {
            if (!wasInputJump)
            {
                state = doubleJumpingState;
                SET_BIT(stateFlags, STATE_FALLING_FLAG_CAN_JUMP_AGAIN, false);
                stateTime = 0;
                vel._y = speedY;
            }
            else if (stateTime < numJumpFrames)
            {
                vel._y = speedY;
            }
        }
    }
    else if (state == fallingState)
    {
        if (isInputJump &&
            !wasInputJump &&
            IS_BIT_SET(stateFlags, STATE_FALLING_FLAG_CAN_JUMP_AGAIN))
        {
            state = doubleJumpingState;
            SET_BIT(stateFlags, STATE_FALLING_FLAG_CAN_JUMP_AGAIN, false);
            stateTime = 0;
            vel._y = speedY;
        }
    }
    else if (isGrounded)
    {
        if (isInputJump &&
            !wasInputJump)
        {
            state = jumpingState;
            SET_BIT(stateFlags, STATE_FALLING_FLAG_CAN_JUMP_AGAIN, true);
            stateTime = 0;
            vel._y = speedY;
            
            if (isVampire())
            {
                vel._x = 0;
            }
        }
    }
}

void JonController::processAttackInput(uint16_t inputState)
{
    Entity& e = *_entity;
    
    bool isInputAttack = IS_BIT_SET(inputState, JISF_EXECUTING_ATTACK);
    bool wasInputAttack = IS_BIT_SET(e.lastProcessedInputState(), JISF_EXECUTING_ATTACK);
    
    uint8_t& state = e.state()._state;
    uint16_t& stateTime = e.state()._stateTime;
    
    if (isRabbit() &&
        state != STATE_PUNCHING &&
        isInputAttack &&
        !wasInputAttack)
    {
        state = STATE_PUNCHING;
        stateTime = 0;
    }
}

void JonController::processAbilityInput(uint16_t inputState)
{
    Entity& e = *_entity;
    
    bool isInputAbility = IS_BIT_SET(inputState, JISF_EXECUTING_ABILITY);
    bool wasInputAbility = IS_BIT_SET(e.lastProcessedInputState(), JISF_EXECUTING_ABILITY);
    
    uint8_t& state = e.state()._state;
    uint16_t& stateTime = e.state()._stateTime;
    Vector2& vel = e.velocity();
    bool isGrounded = e.isGrounded();
    
    if (isRabbit())
    {
        if (isGrounded)
        {
            if (state != STATE_DRILLING &&
                isInputAbility &&
                !wasInputAbility)
            {
                state = STATE_DRILLING;
                stateTime = 0;
            }
        }
        else
        {
            if (state != STATE_DRILL_STOMPING &&
                isInputAbility &&
                !wasInputAbility)
            {
                state = STATE_DRILL_STOMPING;
                stateTime = 0;
                e.pose()._isZeroGravity = true;
                vel._x = 0;
                vel._y = 0;
            }
        }
    }
    else
    {
        if (state != STATE_VAMPIRE_DASHING &&
            isInputAbility &&
            !wasInputAbility)
        {
            static float speedX = e.data().getFloat("speedVampireX");
            
            state = STATE_VAMPIRE_DASHING;
            stateTime = 0;
            e.pose()._isZeroGravity = true;
            float dashSpeedX = e.pose()._isXFlipped ? -speedX : speedX;
            vel._x = dashSpeedX * 2;
            vel._y = 0;
        }
    }
}

void JonController::processWarmingUpInput(uint16_t inputState)
{
    Entity& e = *_entity;
    
    bool isInputWarmingUp = IS_BIT_SET(inputState, JISF_WARMING_UP);
    bool wasInputWarmingUp = IS_BIT_SET(e.lastProcessedInputState(), JISF_WARMING_UP);
    
    uint8_t& state = e.state()._state;
    uint16_t& stateTime = e.state()._stateTime;
    
    if (isRabbit() &&
        state != STATE_WARMING_UP &&
        isInputWarmingUp &&
        !wasInputWarmingUp)
    {
        state = STATE_WARMING_UP;
        stateTime = 0;
    }
}

void JonController::processSpecialInput(uint16_t inputState)
{
    Entity& e = *_entity;
    
    bool isInputSpecial = IS_BIT_SET(inputState, JISF_TRIGGERING_SPECIAL);
    bool wasInputSpecial = IS_BIT_SET(e.lastProcessedInputState(), JISF_TRIGGERING_SPECIAL);
    
    uint8_t& state = e.state()._state;
    uint16_t& stateTime = e.state()._stateTime;
    
    State attemptingToTransformState = isRabbit() ? STATE_ATTEMPTING_TO_TRANSFORM : STATE_ATTEMPTING_TO_REVERT;
    State cancelingAttemptState = isRabbit() ? STATE_CANCELING_ATTEMPT_TO_TRANSFORM : STATE_CANCELING_ATTEMPT_TO_REVERT;
    
    if (state == attemptingToTransformState)
    {
        if (!isInputSpecial)
        {
            // TODO, shouldn't need a canceling state in order to trigger a canceling sound.
            // ideally, if the player stops holding the transform/revert button,
            // they just go back to whatever state is most appropriate (e.g. fall, run)
            // as for the sound, there should be a way to define a sound to be played when a state disappears from one frame to the next before reaching its final frame
            state = cancelingAttemptState;
            stateTime = 0;
            e.pose()._isSlowed = false;
            e.pose()._isZeroGravity = false;
        }
    }
    else
    {
        if (isInputSpecial &&
            !wasInputSpecial)
        {
            state = attemptingToTransformState;
            stateTime = 0;
            e.pose()._isSlowed = true;
        }
    }
}

bool JonController::isRabbit()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    
    return state == STATE_IDLE ||
    state == STATE_RUNNING ||
    state == STATE_JUMPING ||
    state == STATE_FALLING ||
    state == STATE_DOUBLE_JUMPING ||
    state == STATE_LANDING ||
    state == STATE_DRILL_STOMPING ||
    state == STATE_LANDING_ROLL ||
    state == STATE_WARMING_UP ||
    state == STATE_PUNCHING ||
    state == STATE_DRILLING ||
    state == STATE_ATTEMPTING_TO_TRANSFORM ||
    state == STATE_CANCELING_ATTEMPT_TO_TRANSFORM ||
    state == STATE_REVERTING ||
    state == STATE_RABBIT_DEATH;
}

bool JonController::isReleasingShockwave()
{
    return _isReleasingShockwave;
}

uint16_t JonController::shockwaveStateTime()
{
    return _shockwaveStateTime;
}

bool JonController::isVampire()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    
    return state == STATE_TRANSFORMING ||
    state == STATE_VAMPIRE_IDLE ||
    state == STATE_VAMPIRE_RUNNING ||
    state == STATE_VAMPIRE_JUMPING ||
    state == STATE_VAMPIRE_DASHING ||
    state == STATE_VAMPIRE_DOUBLE_JUMPING ||
    state == STATE_VAMPIRE_GLIDING ||
    state == STATE_VAMPIRE_FALLING ||
    state == STATE_VAMPIRE_LANDING ||
    state == STATE_ATTEMPTING_TO_REVERT ||
    state == STATE_CANCELING_ATTEMPT_TO_REVERT ||
    state == STATE_VAMPIRE_DASH_COMPLETED ||
    state == STATE_VAMPIRE_DEATH;
}

bool JonController::isMovementInputAllowed()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    
    return state == STATE_IDLE ||
    state == STATE_RUNNING ||
    state == STATE_JUMPING ||
    state == STATE_FALLING ||
    state == STATE_DOUBLE_JUMPING ||
    state == STATE_LANDING ||
    state == STATE_WARMING_UP ||
    state == STATE_VAMPIRE_IDLE ||
    state == STATE_VAMPIRE_RUNNING ||
    state == STATE_VAMPIRE_DOUBLE_JUMPING ||
    state == STATE_VAMPIRE_GLIDING ||
    state == STATE_VAMPIRE_FALLING ||
    state == STATE_VAMPIRE_LANDING;
}

bool JonController::isJumpInputAllowed()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    
    return state == STATE_IDLE ||
    state == STATE_RUNNING ||
    state == STATE_JUMPING ||
    state == STATE_FALLING ||
    state == STATE_DOUBLE_JUMPING ||
    state == STATE_LANDING ||
    state == STATE_DRILL_STOMPING ||
    state == STATE_LANDING_ROLL ||
    state == STATE_WARMING_UP ||
    state == STATE_VAMPIRE_IDLE ||
    state == STATE_VAMPIRE_RUNNING ||
    state == STATE_VAMPIRE_JUMPING ||
    state == STATE_VAMPIRE_DOUBLE_JUMPING ||
    state == STATE_VAMPIRE_GLIDING ||
    state == STATE_VAMPIRE_FALLING ||
    state == STATE_VAMPIRE_LANDING;
}

bool JonController::isAttackInputAllowed()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    
    return state == STATE_IDLE ||
    state == STATE_RUNNING ||
    state == STATE_JUMPING ||
    state == STATE_FALLING ||
    state == STATE_DOUBLE_JUMPING ||
    state == STATE_WARMING_UP;
}

bool JonController::isAbilityInputAllowed()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    
    return state == STATE_IDLE ||
    state == STATE_RUNNING ||
    state == STATE_JUMPING ||
    state == STATE_FALLING ||
    state == STATE_DOUBLE_JUMPING ||
    state == STATE_LANDING ||
    state == STATE_WARMING_UP ||
    state == STATE_VAMPIRE_IDLE ||
    state == STATE_VAMPIRE_RUNNING ||
    state == STATE_VAMPIRE_JUMPING ||
    state == STATE_VAMPIRE_DOUBLE_JUMPING ||
    state == STATE_VAMPIRE_GLIDING ||
    state == STATE_VAMPIRE_FALLING ||
    state == STATE_VAMPIRE_LANDING;
}

bool JonController::isWarmingUpInputAllowed()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    
    return state == STATE_IDLE;
}

bool JonController::isSpecialInputAllowed()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    
    return state != STATE_TRANSFORMING &&
    state != STATE_REVERTING;
}

bool JonController::needsStateChangeForMovement()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    
    return state == STATE_IDLE ||
    state == STATE_WARMING_UP ||
    state == STATE_VAMPIRE_IDLE;
}

bool JonController::canCurrentStateBeInterruptedByFalling()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    
    return state == STATE_IDLE ||
    state == STATE_RUNNING ||
    state == STATE_JUMPING ||
    state == STATE_DOUBLE_JUMPING ||
    state == STATE_LANDING ||
    state == STATE_LANDING_ROLL ||
    state == STATE_WARMING_UP ||
    state == STATE_DRILLING ||
    state == STATE_VAMPIRE_IDLE ||
    state == STATE_VAMPIRE_RUNNING ||
    state == STATE_VAMPIRE_JUMPING ||
    state == STATE_VAMPIRE_DOUBLE_JUMPING ||
    state == STATE_VAMPIRE_LANDING ||
    state == STATE_VAMPIRE_DASH_COMPLETED;
}

bool JonController::canCurrentStateBeInterruptedByLanding()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    
    return state == STATE_FALLING ||
    state == STATE_VAMPIRE_GLIDING ||
    state == STATE_VAMPIRE_FALLING;
}

bool JonController::needsToResolveNewStateOnceAnimationEnds()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    
    return state == STATE_JUMPING ||
    state == STATE_DOUBLE_JUMPING ||
    state == STATE_LANDING ||
    state == STATE_LANDING_ROLL ||
    state == STATE_PUNCHING ||
    state == STATE_DRILLING ||
    state == STATE_TRANSFORMING ||
    state == STATE_CANCELING_ATTEMPT_TO_TRANSFORM ||
    state == STATE_DRILLING ||
    state == STATE_VAMPIRE_JUMPING ||
    state == STATE_VAMPIRE_LANDING ||
    state == STATE_REVERTING ||
    state == STATE_CANCELING_ATTEMPT_TO_REVERT ||
    state == STATE_VAMPIRE_DASH_COMPLETED ||
    state == STATE_RABBIT_DEATH ||
    state == STATE_VAMPIRE_DEATH;
}

void JonController::resolveNewState()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    uint8_t& stateFlags = e.state()._stateFlags;
    uint16_t& stateTime = e.state()._stateTime;
    Vector2& vel = e.velocity();
    bool isGrounded = e.isGrounded();
    
    State deathState = isRabbit() ? STATE_RABBIT_DEATH : STATE_VAMPIRE_DEATH;
    State idleState = isRabbit() ? STATE_IDLE : STATE_VAMPIRE_IDLE;
    State doubleJumpingState = isRabbit() ? STATE_DOUBLE_JUMPING : STATE_VAMPIRE_DOUBLE_JUMPING;
    State fallingState = isRabbit() ? STATE_FALLING : STATE_VAMPIRE_FALLING;
    
    if (state == deathState)
    {
        e.exile();
    }
    else if (isGrounded)
    {
        state = idleState;
        stateTime = 0;
        vel._x = 0;
    }
    else if (vel._y < 0)
    {
        bool canJumpAgain = true;
        if (state == doubleJumpingState)
        {
            canJumpAgain = false;
        }
        state = fallingState;
        SET_BIT(stateFlags, STATE_FALLING_FLAG_CAN_JUMP_AGAIN, canJumpAgain);
        stateTime = 0;
    }
}
