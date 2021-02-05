//
//  EntityPhysicsController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

#define DECL_EntityPhysicsController_create         \
public:                                             \
    static EntityPhysicsController* create(Entity* e)

#define IMPL_EntityPhysicsController_create(name)   \
EntityPhysicsController* name::create(Entity* e)    \
{                                                   \
    return new name(e);                             \
}                                                   \

class Entity;

class EntityPhysicsController
{
    DECL_EntityPhysicsController_create;
    
public:
    EntityPhysicsController(Entity* e);
    virtual ~EntityPhysicsController() {}
    
//    virtual void read();
    
protected:
    Entity* _entity;
};
