//
//  EntityPhysicsController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI_NOPARENT(EntityPhysicsController)
IMPL_EntityController_NOPARENT_create(EntityPhysicsController)

EntityPhysicsController::EntityPhysicsController(Entity* e) :
_entity(e)
{
    // Empty
}

EntityPhysicsController::~EntityPhysicsController()
{
    // Empty
}

Vector2 EntityPhysicsController::velocity()
{
    // TODO
    return VECTOR2_ZERO;
}

void EntityPhysicsController::setVelocity(Vector2 v)
{
    // TODO
    UNUSED(v);
}

void EntityPhysicsController::updatePoseFromBody()
{
    // TODO
}

void EntityPhysicsController::updateBodyFromPose()
{
    // TODO
}
