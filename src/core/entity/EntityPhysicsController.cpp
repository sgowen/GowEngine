//
//  EntityPhysicsController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI_NOPARENT(EntityPhysicsController)

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
    return Vector2::ZERO;
}

void EntityPhysicsController::setVelocity(Vector2 v)
{
    UNUSED(v);
}

void EntityPhysicsController::updatePoseFromBody()
{
    // Empty
}

void EntityPhysicsController::updateBodyFromPose()
{
    // Empty
}
