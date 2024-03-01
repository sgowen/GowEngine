//
//  MonsterInputController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/01/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI(MonsterInputController, EntityInputController)
IMPL_EntityInputController_create(MonsterInputController)

MonsterInputController::MonsterInputController(Entity* e) : EntityInputController(e)
{
    // Empty
}

void MonsterInputController::processEvent(uint16_t& inputState, CursorEvent* e)
{
    // Empty
}

void MonsterInputController::processEvent(uint16_t& inputState, GamepadEvent* e)
{
    // Empty
}

void MonsterInputController::processEvent(uint16_t& inputState, KeyboardEvent* e)
{
    // Empty
}

void MonsterInputController::runAI(uint16_t& inputState)
{
    Entity& e = *_entity;
    Vector2& vel = e.velocity();
    uint8_t& state = e.state()._state;
    uint8_t& stateFlags = e.state()._stateFlags;
    uint16_t& stateTime = e.state()._stateTime;
    
    uint32_t touchingEntityID = e.networkDataField("touchingEntityID").valueUInt32();
    Entity* touchingEntity = e.world()->getEntityByID(touchingEntityID);
    SET_BIT(inputState, MISF_EXECUTING_ATTACK, touchingEntity != nullptr && touchingEntity->isPlayer());
    
    float shortestDistance = 16.0f;
    Entity* target = nullptr;
    for (Entity* e : e.world()->getPlayers())
    {
        float distance = e->position().dist(_entity->position());
        if (distance < shortestDistance)
        {
            shortestDistance = distance;
            target = e;
        }
    }
    if (target)
    {
        SET_BIT(inputState, MISF_MOVING_RIGHT, target->position()._x > e.position()._x);
        SET_BIT(inputState, MISF_MOVING_LEFT, target->position()._x < e.position()._x);
    }
    else
    {
        SET_BIT(inputState, MISF_MOVING_RIGHT, false);
        SET_BIT(inputState, MISF_MOVING_LEFT, false);
        SET_BIT(inputState, MISF_EXECUTING_ATTACK, false);
    }
}
