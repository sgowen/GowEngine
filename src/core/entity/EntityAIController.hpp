//
//  EntityAIController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/01/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/RTTI.hpp"

class Entity;

class EntityAIController
{
    DECL_RTTI_NOPARENT;
    DECL_EntityAIController_create;
    
public:
    EntityAIController(Entity* e);
    virtual ~EntityAIController() {}
    
    virtual void runAI(uint16_t& inputState);
    
protected:
    Entity* _entity;
};
