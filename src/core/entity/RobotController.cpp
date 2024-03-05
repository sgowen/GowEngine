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
    
    if (isAttackInputAllowed())
    {
        processAttackInput(inputState);
    }
}

void RobotController::onUpdate(uint32_t numMovesProcessed)
{
    Entity& e = *_entity;
    Vector2& vel = e.velocity();
    uint8_t& state = e.state()._state;
    uint8_t& stateFlags = e.state()._stateFlags;
    uint16_t& stateTime = e.state()._stateTime;
    
    std::string textureMapping = e.controllerRender()->getTextureMapping();
    Animation* animation = ASSETS_MGR.animation(textureMapping);
    uint16_t animationNumFrames = animation == nullptr ? 1 : animation->cycleTime();
    
    if ((state == e.state("PUNCH_1") && stateTime == 6) ||
        (state == e.state("PUNCH_2") && stateTime == 6) ||
        (state == e.state("PUNCH_3") && stateTime == 6))
    {
        uint32_t touchingEntityID = e.networkDataField("touchingEntityID").valueUInt32();
        Entity* touchingEntity = e.world()->getEntityByID(touchingEntityID);
        if (touchingEntity != nullptr)
        {
            touchingEntity->message(MSG_DAMAGE, &e);
            
            if (state == e.state("PUNCH_3"))
            {
                touchingEntity->message(MSG_DAMAGE, &e);
                touchingEntity->message(MSG_DAMAGE, &e);
                touchingEntity->message(MSG_DAMAGE, &e);
                touchingEntity->message(MSG_DAMAGE, &e);
            }
        }
    }
    
    if (stateTime >= animationNumFrames &&
        needsToResolveNewStateOnceAnimationEnds())
    {
        resolveNewState();
    }
}

void RobotController::onMessage(uint16_t message, Entity* fromEntity)
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    uint16_t& stateTime = e.state()._stateTime;
    Vector2& vel = e.velocity();
    
    if (message == MSG_DANGEROUS_TOUCH)
    {
        e.networkDataField("touchingEntityID").setValueUInt32(fromEntity->getID());
    }
    else if (message == MSG_NO_TOUCH)
    {
        e.networkDataField("touchingEntityID").setValueUInt32(0);
    }
    else if (message == MSG_DAMAGE)
    {
        e.exile();
    }
}

void RobotController::processMovementInput(uint16_t inputState)
{
    Entity& e = *_entity;
    
    bool isInputLeft = IS_BIT_SET(inputState, e.inputStateFlag("MOVING_LEFT"));
    bool wasInputLeft = IS_BIT_SET(e.lastProcessedInputState(), e.inputStateFlag("MOVING_LEFT"));
    bool isInputRight = IS_BIT_SET(inputState, e.inputStateFlag("MOVING_RIGHT"));
    bool wasInputRight = IS_BIT_SET(e.lastProcessedInputState(), e.inputStateFlag("MOVING_RIGHT"));
    
    static float speedX = e.data().getFloat("speedX");
    
    uint8_t& state = e.state()._state;
    uint16_t& stateTime = e.state()._stateTime;
    Vector2& vel = e.velocity();
    bool isGrounded = e.isGrounded();
    
    if (isInputLeft)
    {
        if (needsStateChangeForMovement())
        {
            state = e.state("RUNNING");
            stateTime = 0;
        }
        vel._x = -speedX;
        e.pose()._isXFlipped = true;
    }
    else if (isInputRight)
    {
        if (needsStateChangeForMovement())
        {
            state = e.state("RUNNING");
            stateTime = 0;
        }
        vel._x = speedX;
        e.pose()._isXFlipped = false;
    }
    else if (wasInputLeft || wasInputRight)
    {
        if (isGrounded)
        {
            state = e.state("IDLE");
            stateTime = 0;
        }
        vel._x = 0;
    }
}

void RobotController::processJumpInput(uint16_t inputState)
{
    Entity& e = *_entity;
    
    bool isInputJump = IS_BIT_SET(inputState, e.inputStateFlag("JUMPING"));
    bool wasInputJump = IS_BIT_SET(e.lastProcessedInputState(), e.inputStateFlag("JUMPING"));
    
    static uint32_t numJumpFrames = e.data().getUInt("numJumpFrames");
    static float speedY = e.data().getFloat("speedY");
    
    uint8_t& state = e.state()._state;
    uint8_t& stateFlags = e.state()._stateFlags;
    uint16_t& stateTime = e.state()._stateTime;
    Vector2& vel = e.velocity();
    bool isGrounded = e.isGrounded();
    
    if (state == e.state("JUMPING"))
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
            state = e.state("JUMPING");
            stateTime = 0;
            vel._y = speedY;
        }
    }
}

void RobotController::processAttackInput(uint16_t inputState)
{
    Entity& e = *_entity;
    
    bool isInputAttack = IS_BIT_SET(inputState, e.inputStateFlag("EXECUTING_ATTACK"));
    bool wasInputAttack = IS_BIT_SET(e.lastProcessedInputState(), e.inputStateFlag("EXECUTING_ATTACK"));
    
    uint8_t& state = e.state()._state;
    uint16_t& stateTime = e.state()._stateTime;
    
    if (isInputAttack &&
        !wasInputAttack)
    {
        // TODO, the stateTime code is temporary
        // Player shouldn't be able to cut off
        // the first punch animation with the second
        // at least, not right away.
        // This stateTime checker code just prevents
        // rapid tapping from creating rapid jitter
        if (state == e.state("PUNCH_2") && stateTime >= 12)
        {
            state = e.state("PUNCH_3");
            stateTime = 0;
        }
        else if (state == e.state("PUNCH_1") && stateTime >= 12)
        {
            state = e.state("PUNCH_2");
            stateTime = 0;
        }
        else if (state != e.state("PUNCH_1"))
        {
            state = e.state("PUNCH_1");
            stateTime = 0;
        }
    }
}

bool RobotController::isMovementInputAllowed()
{
    Entity& e = *_entity;
    std::string state = e.state(e.state()._state);
    
    return state == "IDLE" ||
    state == "RUNNING" ||
    state == "JUMPING";
}

bool RobotController::isJumpInputAllowed()
{
    Entity& e = *_entity;
    std::string state = e.state(e.state()._state);
    
    return state == "IDLE" ||
    state == "JUMPING" ||
    state == "RUNNING";
}

bool RobotController::isAttackInputAllowed()
{
    Entity& e = *_entity;
    std::string state = e.state(e.state()._state);
    
    return state == "IDLE" ||
    state == "RUNNING" ||
    state == "JUMPING" ||
    state == "PUNCH_1" ||
    state == "PUNCH_2";
}

bool RobotController::needsStateChangeForMovement()
{
    Entity& e = *_entity;
    std::string state = e.state(e.state()._state);
    
    return state == "IDLE";
}

bool RobotController::needsToResolveNewStateOnceAnimationEnds()
{
    Entity& e = *_entity;
    std::string state = e.state(e.state()._state);
    
    return state == "JUMPING" ||
    state == "PUNCH_1" ||
    state == "PUNCH_2" ||
    state == "PUNCH_3";
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
        state = e.state("IDLE");
        stateTime = 0;
        vel._x = 0;
    }
}
