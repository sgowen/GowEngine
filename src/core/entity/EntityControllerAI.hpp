//
//  EntityControllerAI.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/04/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/RTTI.hpp"
#include "core/common/Macros.hpp"

class Entity;

class EntityControllerAI
{
    DECL_RTTI_NOPARENT;
    DECL_EntityControllerAI_create;
    
public:
    EntityControllerAI(Entity* e);
    virtual ~EntityControllerAI() {}
    
    virtual void runAI(uint16_t& inputState);
    
protected:
    Entity* _entity;
};
