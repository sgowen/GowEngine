//
//  MonsterController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/24/24.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI(MonsterController, EntityController)
IMPL_EntityController_create(MonsterController)

MonsterController::MonsterController(Entity* e) : EntityController(e)
{
    e->networkDataField("health").setValueUInt8(7);
}

void MonsterController::processInput(uint16_t inputState)
{
    if (isMovementInputAllowed())
    {
        processMovementInput(inputState);
    }
    
    if (isAttackInputAllowed())
    {
        processAttackInput(inputState);
    }
}

void MonsterController::onUpdate(uint32_t numMovesProcessed)
{
    Entity& e = *_entity;
    Vector2& vel = e.velocity();
    uint8_t& state = e.state()._state;
    uint8_t& stateFlags = e.state()._stateFlags;
    uint16_t& stateTime = e.state()._stateTime;
    
    std::string textureMapping = e.renderController()->getTextureMapping();
    Animation* animation = ASSETS_MGR.animation(textureMapping);
    uint16_t animationNumFrames = animation == nullptr ? 1 : animation->cycleTime();
    
    if (state == STATE_PREPARING_ATTACK && stateTime == 60)
    {
        state = STATE_ATTACKING;
        stateTime = 0;
    }
    else if (state == STATE_ATTACKING && stateTime == 30)
    {
        uint32_t touchingEntityID = e.networkDataField("touchingEntityID").valueUInt32();
        Entity* touchingEntity = e.world()->getEntityByID(touchingEntityID);
        if (touchingEntity != nullptr && touchingEntity->isPlayer())
        {
            touchingEntity->message(MSG_DAMAGE, &e);
        }
    }
    
    if (stateTime >= animationNumFrames &&
        needsToResolveNewStateOnceAnimationEnds())
    {
        resolveNewState();
    }
}

void MonsterController::onMessage(uint16_t message, Entity* fromEntity)
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    uint16_t& stateTime = e.state()._stateTime;
    Vector2& vel = e.velocity();
    
    if (message == MSG_DANGEROUS_TOUCH)
    {
        if (fromEntity->isPlayer())
        {
            e.networkDataField("touchingEntityID").setValueUInt32(fromEntity->getID());
        }
    }
    else if (message == MSG_NO_TOUCH)
    {
        if (fromEntity->isPlayer())
        {
            e.networkDataField("touchingEntityID").setValueUInt32(0);
        }
    }
    else if (message == MSG_DAMAGE &&
        state != STATE_DYING)
    {
        uint8_t health = e.networkDataField("health").valueUInt8();
        health -= 1;
        e.networkDataField("health").setValueUInt8(health);
        if (health == 0)
        {
            state = STATE_DYING;
            stateTime = 0;
            vel.reset();
        }
    }
}

void MonsterController::processMovementInput(uint16_t inputState)
{
    Entity& e = *_entity;
    
    bool isInputLeft = IS_BIT_SET(inputState, MISF_MOVING_LEFT);
    bool wasInputLeft = IS_BIT_SET(e.lastProcessedInputState(), MISF_MOVING_LEFT);
    bool isInputRight = IS_BIT_SET(inputState, MISF_MOVING_RIGHT);
    bool wasInputRight = IS_BIT_SET(e.lastProcessedInputState(), MISF_MOVING_RIGHT);
    
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
    else
    {
        if (isGrounded)
        {
            state = STATE_IDLE;
            stateTime = 0;
        }
        vel._x = 0;
    }
}

void MonsterController::processAttackInput(uint16_t inputState)
{
    Entity& e = *_entity;
    
    bool isInputAttack = IS_BIT_SET(inputState, MISF_EXECUTING_ATTACK);
    bool wasInputAttack = IS_BIT_SET(e.lastProcessedInputState(), MISF_EXECUTING_ATTACK);
    
    uint8_t& state = e.state()._state;
    uint16_t& stateTime = e.state()._stateTime;
    
    if (isInputAttack &&
        !wasInputAttack)
    {
        state = STATE_PREPARING_ATTACK;
        stateTime = 0;
    }
}

bool MonsterController::isMovementInputAllowed()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    
    return state == STATE_IDLE ||
    state == STATE_RUNNING ||
    state == STATE_PREPARING_ATTACK ||
    state == STATE_ATTACKING;
}

bool MonsterController::isAttackInputAllowed()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    
    return state == STATE_IDLE ||
    state == STATE_RUNNING;
}

bool MonsterController::needsStateChangeForMovement()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    
    return state == STATE_IDLE;
}

bool MonsterController::needsToResolveNewStateOnceAnimationEnds()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    
    return state == STATE_ATTACKING ||
    state == STATE_DYING;
}

void MonsterController::resolveNewState()
{
    Entity& e = *_entity;
    uint8_t& state = e.state()._state;
    uint8_t& stateFlags = e.state()._stateFlags;
    uint16_t& stateTime = e.state()._stateTime;
    Vector2& vel = e.velocity();
    bool isGrounded = e.isGrounded();
    
    if (state == STATE_DYING)
    {
        e.exile();
    }
    else if (isGrounded)
    {
        state = STATE_IDLE;
        stateTime = 0;
        vel._x = 0;
    }
}
