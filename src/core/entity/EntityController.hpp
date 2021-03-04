//
//  EntityController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/5/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "RTTI.hpp"
#include "Macros.hpp"

class Entity;

class EntityController
{
    DECL_RTTI_NOPARENT;
    DECL_EntityController_create(EntityController);
    
public:
    EntityController(Entity* e);
    virtual ~EntityController() {}
    
    virtual void update() {}
    virtual void onMessage(uint16_t message) {}
    virtual void onCollision(Entity* e) {}
    
protected:
    Entity* _entity;
};
