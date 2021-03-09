//
//  EntityPhysicsController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "EntityPhysicsController.hpp"

#include "Entity.hpp"

IMPL_RTTI_NOPARENT(EntityPhysicsController)
IMPL_EntityController_NOPARENT_create(EntityPhysicsController)

EntityPhysicsController::EntityPhysicsController(Entity* e) :
_entity(e)
{
    // Empty
}
