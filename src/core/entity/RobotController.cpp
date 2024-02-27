//
//  RobotController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/24/24.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI(RobotController, EntityController)
IMPL_EntityController_create(RobotController)

RobotController::RobotController(Entity* e) : EntityController(e)
{
    // Empty
}

void RobotController::processInput(uint16_t inputState)
{
    if (isMovementInputAllowed())
    {
        processMovementInput(inputState);
    }
    
    if (isJumpInputAllowed())
    {
        processJumpInput(inputState);
    }
}

void RobotController::onUpdate(uint32_t numMovesProcessed)
{
    Entity& e = *_entity;
    Vector2& vel = e.velocity();
    uint8_t& state = e.state()._state;
    uint8_t& stateFlags = e.state()._stateFlags;
    uint16_t& stateTime = e.state()._stateTime;
    
    std::string textureMapping = e.renderController()->getTextureMapping();
    Animation* animation = ASSETS_MGR.animation(textureMapping);
    uint16_t animationNumFrames = animation == nullptr ? 1 : animation->cycleTime();
    
    if (stateTime >= animationNumFrames &&
        needsToResolveNewStateOnceAnimationEnds())
    {
        resolveNewState();
    }
}

void RobotController::onMessage(uint16_t message)
{
    // TODO
}

void RobotController::processMovementInput(uint16_t inputState)
{
    Entity& e = *_entity;
    
    bool isInputLeft = IS_BIT_SET(inputState, RISF_MOVING_LEFT);
    bool wasInputLeft = IS_BIT_SET(e.lastProcessedInputState(), RISF_MOVING_LEFT);
    bool isInputRight = IS_BIT_SET(inputState, RISF_MOVING_RIGHT);
    bool wasInputRight = IS_BIT_SET(e.lastProcessedInputState(), RISF_MOVING_RIGHT);
    
    static float speedX = e.data().getFloat("speedX");
    
    uint8_t& state = e.state()._state;
    uint16_t& stateTime = e.state()._stateTime;
    Vector2& vel = e.velocity();
    bool isGrounded = e.isGrounded();
    
    if (isInputLeft)
    {
        if (needsStateChangeForMovement())
        {
            state = STATE_RUNNING;
            stateTime = 0;
        }
        vel._x = -speedX;
        e.pose()._isXFlipped = true;
    }
    else if (isInputRight)
    {
        if (needsStateChangeForMovement())
        {
            state = STATE_RUNNING;
            stateTime = 0;
        }
        vel._x = speedX;
        e.pose()._isXFlipped = false;
    }
    else if (wasInputLeft || wasInputRight)
    {
        if (isGrounded)
        {
            state = STATE_IDLE;
            stateTime = 0;
        }
        vel._x = 0;
    }
}

void RobotController::processJumpInput(uint16_t inputState)
{
    Entity& e = *_entity;
    
    bool isInputJump = IS_BIT_SET(inputState, RISF_JUMPING);
    bool wasInputJump = IS_BIT_SET(e.lastProcessedInputState(), RISF_JUMPING);
    
    static uint32_t numJumpFrames = e.data().getUInt("numJumpFrames");
    static float speedY = e.data().getFloat("speedY");
    
    uint8_t& state = e.state()._state;
    uint8_t& stateFlags = e.state()._stateFlags;
    uint16_t& stateTime = e.state()._stateTime;
    Vector2& vel = e.velocity();
    bool isGrounded = e.isGrounded();
    
    if (state == STATE_JUMPING)
    {
        if (isInputJump)
        {
            if (stateTime < numJumpFrames)
            {
                vel._y = speedY;
            }
        }
    }
    else if (isGrounded)
    {
        if (isInputJump &&
            !wasInputJump)
        {
            state = STATE_JUMPING;
            stateTime = 0;
            vel._y = speedY;
        }
    }
}

bool RobotController::isMovementInputAllowed()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    
    return state == STATE_IDLE ||
    state == STATE_RUNNING ||
    state == STATE_JUMPING;
}

bool RobotController::isJumpInputAllowed()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    
    return state == STATE_IDLE ||
    state == STATE_JUMPING ||
    state == STATE_RUNNING;
}

bool RobotController::needsStateChangeForMovement()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    
    return state == STATE_IDLE;
}

bool RobotController::canCurrentStateBeInterruptedByLanding()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    
    return state == STATE_JUMPING;
}

bool RobotController::needsToResolveNewStateOnceAnimationEnds()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    
    return state == STATE_JUMPING;
}

void RobotController::resolveNewState()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    uint8_t& stateFlags = e.state()._stateFlags;
    uint16_t& stateTime = e.state()._stateTime;
    Vector2& vel = e.velocity();
    bool isGrounded = e.isGrounded();
    
    if (isGrounded)
    {
        state = STATE_IDLE;
        stateTime = 0;
        vel._x = 0;
    }
}
