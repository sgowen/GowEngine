//
//  EntityController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/5/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#define DECL_EntityController_create                  \
public:                                               \
    static EntityController* create(Entity* e)

#define IMPL_EntityController_create(type)            \
EntityController* type::create(Entity* e)             \
{                                                     \
    return new type(e);                               \
}

#define IMPL_EntityController_create_NOPARENT         \
IMPL_EntityController_create(EntityController)

#include "core/common/RTTI.hpp"

class Entity;

class EntityController
{
    DECL_RTTI_NOPARENT;
    DECL_EntityController_create;
    
public:
    EntityController(Entity* e);
    virtual ~EntityController() {}
    
    virtual void onUpdate() {}
    virtual void onMessage(uint16_t message) {}
    virtual void onCollision(Entity* e) {}
    
protected:
    Entity* _entity;
};
