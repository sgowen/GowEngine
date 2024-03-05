//
//  EntityControllerAI.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/04/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI_NOPARENT(EntityControllerAI)
IMPL_EntityControllerAI_create_NOPARENT

EntityControllerAI::EntityControllerAI(Entity* e) :
_entity(e)
{
    // Empty
}

void EntityControllerAI::runAI(uint16_t& inputState)
{
    // Empty
}
