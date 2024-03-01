//
//  EntityInputController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/01/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI_NOPARENT(EntityInputController)
IMPL_EntityInputController_create_NOPARENT

EntityInputController::EntityInputController(Entity* e) :
_entity(e)
{
    // Empty
}

void EntityInputController::processEvent(uint16_t& inputState, CursorEvent* gpe)
{
    // Empty
}

void EntityInputController::processEvent(uint16_t& inputState, GamepadEvent* gpe)
{
    // Empty
}

void EntityInputController::processEvent(uint16_t& inputState, KeyboardEvent* gpe)
{
    // Empty
}

void EntityInputController::runAI(uint16_t& inputState)
{
    // Empty
}
