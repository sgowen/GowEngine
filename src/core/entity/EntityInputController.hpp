//
//  EntityInputController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/01/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/RTTI.hpp"

class Entity;

class EntityInputController
{
    DECL_RTTI_NOPARENT;
    
public:
    EntityInputController(Entity* e);
    virtual ~EntityInputController();
    
    virtual void updateBodyFromPose() = 0;
    
protected:
    Entity* _entity;
};
