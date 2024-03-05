//
//  EntityControllerPhysics.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/RTTI.hpp"
#include "core/math/Vector2.hpp"

class Entity;

class EntityControllerPhysics
{
    DECL_RTTI_NOPARENT;
    
public:
    EntityControllerPhysics(Entity* e);
    virtual ~EntityControllerPhysics() {}
    
    virtual void updateBodyFromPose() = 0;
    
protected:
    Entity* _entity;
};
