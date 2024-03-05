//
//  EntityController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/5/18.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/RTTI.hpp"
#include "core/common/Macros.hpp"

class Entity;

class EntityController
{
    DECL_RTTI_NOPARENT;
    DECL_EntityController_create;
    
public:
    EntityController(Entity* e);
    virtual ~EntityController() {}
    
    virtual void processInput(uint16_t inputState);
    virtual void onUpdate(uint32_t numMovesProcessed);
    virtual void onMessage(uint16_t message, Entity* fromEntity);
    
protected:
    Entity* _entity;
};
