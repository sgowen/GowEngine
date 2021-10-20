//
//  NosPhysicsController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI(NosPhysicsController, EntityPhysicsController)

NosPhysicsController::NosPhysicsController(Entity* e) : EntityPhysicsController(e),
_isBodyFacingLeft(false)
{
    // Empty
}

NosPhysicsController::~NosPhysicsController()
{
    // Empty
}

Vector2 NosPhysicsController::velocity()
{
    return Vector2::ZERO;
}

void NosPhysicsController::setVelocity(Vector2 v)
{
    // TODO
}

void NosPhysicsController::updatePoseFromBody()
{
    // TODO
}

void NosPhysicsController::updateBodyFromPose()
{
    // TODO
}
