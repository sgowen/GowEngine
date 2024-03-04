//
//  EntityAIController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/04/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI_NOPARENT(EntityAIController)
IMPL_EntityAIController_create_NOPARENT

EntityAIController::EntityAIController(Entity* e) :
_entity(e)
{
    // Empty
}

void EntityAIController::runAI(uint16_t& inputState)
{
    // Empty
}
