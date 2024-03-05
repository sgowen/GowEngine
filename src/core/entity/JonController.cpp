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

JonController::JonController(Entity* e) : EntityController(e)
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
    
    std::string textureMapping = e.controllerRender()->getTextureMapping();
    Animation* animation = ASSETS_MGR.animation(textureMapping);
    uint16_t animationNumFrames = animation == nullptr ? 1 : animation->cycleTime();
    uint16_t animationNumFramesBeforeFirstLoopingFrame = animation == nullptr ? 1 : animation->cycleTimeBeforeFirstLoopingFrame();
    
    uint16_t shockwaveStartTime = e.networkDataField("shockwaveStartTime").valueUInt16();
    bool isReleasingShockwave = e.networkDataField("isReleasingShockwave").valueBool();
    
    uint16_t shockwaveStateTime = numMovesProcessed - shockwaveStartTime;
    e.networkDataField("shockwaveStateTime").setValueUInt16(shockwaveStateTime);
    if (isReleasingShockwave &&
        shockwaveStateTime > 120)
    {
        e.networkDataField("isReleasingShockwave").setValueBool(false);
    }
    
    // Rabbit
    if (state == e.state("DRILL_STOMPING"))
    {
        if (e.isGrounded())
        {
            static float speedX = e.data().getFloat("speedX");
            
            state = e.state("LANDING_ROLL");
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
    
    if (state == e.state("ATTEMPTING_TO_TRANSFORM") &&
        stateTime == animationNumFrames)
    {
        state = e.state("TRANSFORMING");
        stateTime = 0;
        // TODO, figure out some way to get rid of local data in the Controller
        // Anything not networked is risky.
        e.networkDataField("isReleasingShockwave").setValueBool(true);
        e.networkDataField("shockwaveStartTime").setValueUInt16(numMovesProcessed);
        e.pose()._isSlowed = false;
        e.pose()._isZeroGravity = false;
    }
    
    // Vampire
    if (state == e.state("VAMPIRE_DOUBLE_JUMPING") &&
        stateTime == animationNumFrames)
    {
        state = e.state("VAMPIRE_GLIDING");
        stateTime = 0;
    }
    
    if (state == e.state("VAMPIRE_DASHING"))
    {
        if (stateTime == animationNumFrames)
        {
            state = e.state("VAMPIRE_DASH_COMPLETED");
            stateTime = 0;
            vel._x = 0;
            e.pose()._isZeroGravity = false;
        }
    }
    
    if (state == e.state("ATTEMPTING_TO_REVERT") &&
        stateTime == animationNumFrames)
    {
        state = e.state("REVERTING");
        stateTime = 0;
        e.networkDataField("isReleasingShockwave").setValueBool(true);
        e.networkDataField("shockwaveStartTime").setValueUInt16(numMovesProcessed);
        e.pose()._isSlowed = false;
        e.pose()._isZeroGravity = false;
    }
    
    // Both Rabbit and Vampire
    if (
        canCurrentStateBeInterruptedByFalling() &&
        !e.isGrounded() &&
        vel._y < 0)
    {
        uint8_t doubleJumpingState = isRabbit() ? e.state("DOUBLE_JUMPING") : e.state("VAMPIRE_DOUBLE_JUMPING");
        uint8_t fallingState = isRabbit() ? e.state("FALLING") : e.state("VAMPIRE_FALLING");
        
        bool canJumpAgain = true;
        if (state == doubleJumpingState)
        {
            canJumpAgain = false;
        }
        state = fallingState;
        SET_BIT(stateFlags, e.stateFlag("FALLING_FLAG_CAN_JUMP_AGAIN"), canJumpAgain);
        stateTime = 0;
    }
    
    if (canCurrentStateBeInterruptedByLanding() &&
        e.isGrounded())
    {
        uint8_t landingState = isRabbit() ? e.state("LANDING") : e.state("VAMPIRE_LANDING");
        
        state = landingState;
        SET_BIT(stateFlags, e.stateFlag("FALLING_FLAG_CAN_JUMP_AGAIN"), true);
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
    
    uint8_t deathState = isRabbit() ? e.state("RABBIT_DEATH") : e.state("VAMPIRE_DEATH");
    
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
    
    bool isInputLeft = IS_BIT_SET(inputState, e.inputStateFlag("MOVING_LEFT"));
    bool wasInputLeft = IS_BIT_SET(e.lastProcessedInputState(), e.inputStateFlag("MOVING_LEFT"));
    bool isInputRight = IS_BIT_SET(inputState, e.inputStateFlag("MOVING_RIGHT"));
    bool wasInputRight = IS_BIT_SET(e.lastProcessedInputState(), e.inputStateFlag("MOVING_RIGHT"));
    
    static float speedRabbitX = e.data().getFloat("speedX");
    static float speedVampireX = e.data().getFloat("speedVampireX");
    float speedX = isRabbit() ? speedRabbitX : speedVampireX;
    
    uint8_t& state = e.state()._state;
    uint16_t& stateTime = e.state()._stateTime;
    Vector2& vel = e.velocity();
    bool isGrounded = e.isGrounded();
    
    uint8_t idleState = isRabbit() ? e.state("IDLE") : e.state("VAMPIRE_IDLE");
    uint8_t runningState = isRabbit() ? e.state("RUNNING") : e.state("VAMPIRE_RUNNING");
    
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
    
    bool isInputJump = IS_BIT_SET(inputState, e.inputStateFlag("JUMPING"));
    bool wasInputJump = IS_BIT_SET(e.lastProcessedInputState(), e.inputStateFlag("JUMPING"));
    
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
    
    uint8_t jumpingState = isRabbit() ? e.state("JUMPING") : e.state("VAMPIRE_JUMPING");
    uint8_t doubleJumpingState = isRabbit() ? e.state("DOUBLE_JUMPING") : e.state("VAMPIRE_DOUBLE_JUMPING");
    uint8_t fallingState = isRabbit() ? e.state("FALLING") : e.state("VAMPIRE_FALLING");
    
    if (state == e.state("VAMPIRE_GLIDING"))
    {
        if (!isInputJump)
        {
            state = e.state("VAMPIRE_FALLING");
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
                SET_BIT(stateFlags, e.stateFlag("FALLING_FLAG_CAN_JUMP_AGAIN"), false);
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
            IS_BIT_SET(stateFlags, e.stateFlag("FALLING_FLAG_CAN_JUMP_AGAIN")))
        {
            state = doubleJumpingState;
            SET_BIT(stateFlags, e.stateFlag("FALLING_FLAG_CAN_JUMP_AGAIN"), false);
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
            SET_BIT(stateFlags, e.stateFlag("FALLING_FLAG_CAN_JUMP_AGAIN"), true);
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
    
    bool isInputAttack = IS_BIT_SET(inputState, e.inputStateFlag("ATTACKING"));
    bool wasInputAttack = IS_BIT_SET(e.lastProcessedInputState(), e.inputStateFlag("ATTACKING"));
    
    uint8_t& state = e.state()._state;
    uint16_t& stateTime = e.state()._stateTime;
    
    if (isRabbit() &&
        state != e.state("PUNCHING") &&
        isInputAttack &&
        !wasInputAttack)
    {
        state = e.state("PUNCHING");
        stateTime = 0;
    }
}

void JonController::processAbilityInput(uint16_t inputState)
{
    Entity& e = *_entity;
    
    bool isInputAbility = IS_BIT_SET(inputState, e.inputStateFlag("EXECUTING_ABILITY"));
    bool wasInputAbility = IS_BIT_SET(e.lastProcessedInputState(), e.inputStateFlag("EXECUTING_ABILITY"));
    
    uint8_t& state = e.state()._state;
    uint16_t& stateTime = e.state()._stateTime;
    Vector2& vel = e.velocity();
    bool isGrounded = e.isGrounded();
    
    if (isRabbit())
    {
        if (isGrounded)
        {
            if (state != e.state("DRILLING") &&
                isInputAbility &&
                !wasInputAbility)
            {
                state = e.state("DRILLING");
                stateTime = 0;
            }
        }
        else
        {
            if (state != e.state("DRILL_STOMPING") &&
                isInputAbility &&
                !wasInputAbility)
            {
                state = e.state("DRILL_STOMPING");
                stateTime = 0;
                e.pose()._isZeroGravity = true;
                vel._x = 0;
                vel._y = 0;
            }
        }
    }
    else
    {
        if (state != e.state("VAMPIRE_DASHING") &&
            isInputAbility &&
            !wasInputAbility)
        {
            static float speedX = e.data().getFloat("speedVampireX");
            
            state = e.state("VAMPIRE_DASHING");
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
    
    bool isInputWarmingUp = IS_BIT_SET(inputState, e.inputStateFlag("WARMING_UP"));
    bool wasInputWarmingUp = IS_BIT_SET(e.lastProcessedInputState(), e.inputStateFlag("WARMING_UP"));
    
    uint8_t& state = e.state()._state;
    uint16_t& stateTime = e.state()._stateTime;
    
    if (isRabbit() &&
        state != e.state("WARMING_UP") &&
        isInputWarmingUp &&
        !wasInputWarmingUp)
    {
        state = e.state("WARMING_UP");
        stateTime = 0;
    }
}

void JonController::processSpecialInput(uint16_t inputState)
{
    Entity& e = *_entity;
    
    bool isInputSpecial = IS_BIT_SET(inputState, e.inputStateFlag("TRIGGERING_SPECIAL"));
    bool wasInputSpecial = IS_BIT_SET(e.lastProcessedInputState(), e.inputStateFlag("TRIGGERING_SPECIAL"));
    
    uint8_t& state = e.state()._state;
    uint16_t& stateTime = e.state()._stateTime;
    
    uint8_t attemptingToTransformState = isRabbit() ? e.state("ATTEMPTING_TO_TRANSFORM") : e.state("ATTEMPTING_TO_REVERT");
    uint8_t cancelingAttemptState = isRabbit() ? e.state("CANCELING_ATTEMPT_TO_TRANSFORM") : e.state("CANCELING_ATTEMPT_TO_REVERT");
    
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
    std::string state = e.state(e.state()._state);
    
    return state == "IDLE" ||
    state == "RUNNING" ||
    state == "JUMPING" ||
    state == "FALLING" ||
    state == "DOUBLE_JUMPING" ||
    state == "LANDING" ||
    state == "DRILL_STOMPING" ||
    state == "LANDING_ROLL" ||
    state == "WARMING_UP" ||
    state == "PUNCHING" ||
    state == "DRILLING" ||
    state == "ATTEMPTING_TO_TRANSFORM" ||
    state == "CANCELING_ATTEMPT_TO_TRANSFORM" ||
    state == "REVERTING" ||
    state == "RABBIT_DEATH";
}

bool JonController::isVampire()
{
    Entity& e = *_entity;
    std::string state = e.state(e.state()._state);
    
    return state == "TRANSFORMING" ||
    state == "VAMPIRE_IDLE" ||
    state == "VAMPIRE_RUNNING" ||
    state == "VAMPIRE_JUMPING" ||
    state == "VAMPIRE_DASHING" ||
    state == "VAMPIRE_DOUBLE_JUMPING" ||
    state == "VAMPIRE_GLIDING" ||
    state == "VAMPIRE_FALLING" ||
    state == "VAMPIRE_LANDING" ||
    state == "ATTEMPTING_TO_REVERT" ||
    state == "CANCELING_ATTEMPT_TO_REVERT" ||
    state == "VAMPIRE_DASH_COMPLETED" ||
    state == "VAMPIRE_DEATH";
}

bool JonController::isMovementInputAllowed()
{
    Entity& e = *_entity;
    std::string state = e.state(e.state()._state);
    
    return state == "IDLE" ||
    state == "RUNNING" ||
    state == "JUMPING" ||
    state == "FALLING" ||
    state == "DOUBLE_JUMPING" ||
    state == "LANDING" ||
    state == "WARMING_UP" ||
    state == "VAMPIRE_IDLE" ||
    state == "VAMPIRE_RUNNING" ||
    state == "VAMPIRE_DOUBLE_JUMPING" ||
    state == "VAMPIRE_GLIDING" ||
    state == "VAMPIRE_FALLING" ||
    state == "VAMPIRE_LANDING";
}

bool JonController::isJumpInputAllowed()
{
    Entity& e = *_entity;
    std::string state = e.state(e.state()._state);
    
    return state == "IDLE" ||
    state == "RUNNING" ||
    state == "JUMPING" ||
    state == "FALLING" ||
    state == "DOUBLE_JUMPING" ||
    state == "LANDING" ||
    state == "DRILL_STOMPING" ||
    state == "LANDING_ROLL" ||
    state == "WARMING_UP" ||
    state == "VAMPIRE_IDLE" ||
    state == "VAMPIRE_RUNNING" ||
    state == "VAMPIRE_JUMPING" ||
    state == "VAMPIRE_DOUBLE_JUMPING" ||
    state == "VAMPIRE_GLIDING" ||
    state == "VAMPIRE_FALLING" ||
    state == "VAMPIRE_LANDING";
}

bool JonController::isAttackInputAllowed()
{
    Entity& e = *_entity;
    std::string state = e.state(e.state()._state);
    
    return state == "IDLE" ||
    state == "RUNNING" ||
    state == "JUMPING" ||
    state == "FALLING" ||
    state == "DOUBLE_JUMPING" ||
    state == "WARMING_UP";
}

bool JonController::isAbilityInputAllowed()
{
    Entity& e = *_entity;
    std::string state = e.state(e.state()._state);
    
    return state == "IDLE" ||
    state == "RUNNING" ||
    state == "JUMPING" ||
    state == "FALLING" ||
    state == "DOUBLE_JUMPING" ||
    state == "LANDING" ||
    state == "WARMING_UP" ||
    state == "VAMPIRE_IDLE" ||
    state == "VAMPIRE_RUNNING" ||
    state == "VAMPIRE_JUMPING" ||
    state == "VAMPIRE_DOUBLE_JUMPING" ||
    state == "VAMPIRE_GLIDING" ||
    state == "VAMPIRE_FALLING" ||
    state == "VAMPIRE_LANDING";
}

bool JonController::isWarmingUpInputAllowed()
{
    Entity& e = *_entity;
    std::string state = e.state(e.state()._state);
    
    return state == "IDLE";
}

bool JonController::isSpecialInputAllowed()
{
    Entity& e = *_entity;
    std::string state = e.state(e.state()._state);
    
    return state != "TRANSFORMING" &&
    state != "REVERTING";
}

bool JonController::needsStateChangeForMovement()
{
    Entity& e = *_entity;
    std::string state = e.state(e.state()._state);
    
    return state == "IDLE" ||
    state == "WARMING_UP" ||
    state == "VAMPIRE_IDLE";
}

bool JonController::canCurrentStateBeInterruptedByFalling()
{
    Entity& e = *_entity;
    std::string state = e.state(e.state()._state);
    
    return state == "IDLE" ||
    state == "RUNNING" ||
    state == "JUMPING" ||
    state == "DOUBLE_JUMPING" ||
    state == "LANDING" ||
    state == "LANDING_ROLL" ||
    state == "WARMING_UP" ||
    state == "DRILLING" ||
    state == "VAMPIRE_IDLE" ||
    state == "VAMPIRE_RUNNING" ||
    state == "VAMPIRE_JUMPING" ||
    state == "VAMPIRE_DOUBLE_JUMPING" ||
    state == "VAMPIRE_LANDING" ||
    state == "VAMPIRE_DASH_COMPLETED";
}

bool JonController::canCurrentStateBeInterruptedByLanding()
{
    Entity& e = *_entity;
    std::string state = e.state(e.state()._state);
    
    return state == "FALLING" ||
    state == "VAMPIRE_GLIDING" ||
    state == "VAMPIRE_FALLING";
}

bool JonController::needsToResolveNewStateOnceAnimationEnds()
{
    Entity& e = *_entity;
    std::string state = e.state(e.state()._state);
    
    return state == "JUMPING" ||
    state == "DOUBLE_JUMPING" ||
    state == "LANDING" ||
    state == "LANDING_ROLL" ||
    state == "PUNCHING" ||
    state == "DRILLING" ||
    state == "TRANSFORMING" ||
    state == "CANCELING_ATTEMPT_TO_TRANSFORM" ||
    state == "DRILLING" ||
    state == "VAMPIRE_JUMPING" ||
    state == "VAMPIRE_LANDING" ||
    state == "REVERTING" ||
    state == "CANCELING_ATTEMPT_TO_REVERT" ||
    state == "VAMPIRE_DASH_COMPLETED" ||
    state == "RABBIT_DEATH" ||
    state == "VAMPIRE_DEATH";
}

void JonController::resolveNewState()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    uint8_t& stateFlags = e.state()._stateFlags;
    uint16_t& stateTime = e.state()._stateTime;
    Vector2& vel = e.velocity();
    bool isGrounded = e.isGrounded();
    
    uint8_t deathState = isRabbit() ? e.state("RABBIT_DEATH") : e.state("VAMPIRE_DEATH");
    uint8_t idleState = isRabbit() ? e.state("IDLE") : e.state("VAMPIRE_IDLE");
    uint8_t doubleJumpingState = isRabbit() ? e.state("DOUBLE_JUMPING") : e.state("VAMPIRE_DOUBLE_JUMPING");
    uint8_t fallingState = isRabbit() ? e.state("FALLING") : e.state("VAMPIRE_FALLING");
    
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
        SET_BIT(stateFlags, e.stateFlag("FALLING_FLAG_CAN_JUMP_AGAIN"), canJumpAgain);
        stateTime = 0;
    }
}
