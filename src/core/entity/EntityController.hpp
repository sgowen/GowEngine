//
//  EntityController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/5/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/RTTI.hpp"

class Entity;

class EntityController
{
    DECL_RTTI_NOPARENT;
    DECL_EntityController_create;
    
public:
    EntityController(Entity* e);
    virtual ~EntityController();
    
    virtual void onUpdate();
    virtual void onMessage(uint16_t message);
    
protected:
    Entity* _entity;
};
