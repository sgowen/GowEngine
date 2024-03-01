//
//  EntityController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/5/18.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI_NOPARENT(EntityController)
IMPL_EntityController_create_NOPARENT

EntityController::EntityController(Entity* e) :
_entity(e)
{
    // Empty
}

void EntityController::processInput(uint16_t inputState)
{
    // Empty
}

void EntityController::onUpdate(uint32_t numMovesProcessed)
{
    // Empty
}

void EntityController::onMessage(uint16_t message, Entity* fromEntity)
{
    // Empty
}
