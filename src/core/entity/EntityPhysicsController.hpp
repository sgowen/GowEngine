//
//  EntityPhysicsController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/RTTI.hpp"
#include "core/math/Vector2.hpp"

class Entity;

class EntityPhysicsController
{
    DECL_RTTI_NOPARENT;
    
public:
    EntityPhysicsController(Entity* e);
    virtual ~EntityPhysicsController();
    
    virtual Vector2 velocity();
    virtual void setVelocity(Vector2 v);
    virtual void updatePoseFromBody();
    virtual void updateBodyFromPose();
    
protected:
    Entity* _entity;
};
