//
//  EntityPhysicsController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#define DECL_EntityPhysicsController_create                  \
public:                                               \
    static EntityPhysicsController* create(Entity* e)

#define IMPL_EntityPhysicsController_create(type)            \
EntityPhysicsController* type::create(Entity* e)             \
{                                                     \
    return new type(e);                               \
}

#define IMPL_EntityPhysicsController_create_NOPARENT         \
IMPL_EntityPhysicsController_create(EntityPhysicsController)

#include "core/common/RTTI.hpp"
#include "core/math/Vector2.hpp"

class Entity;

class EntityPhysicsController
{
    DECL_RTTI_NOPARENT;
    DECL_EntityPhysicsController_create;
    
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
