//
//  EntityPhysicsController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/RTTI.hpp"
#include "core/math/Vector2.hpp"

class Entity;

class EntityPhysicsController
{
    DECL_RTTI_NOPARENT;
    
public:
    EntityPhysicsController(Entity* e);
    virtual ~EntityPhysicsController();
    
    virtual void updatePoseFromBody() = 0;
    virtual void updateBodyFromPose() = 0;
    
protected:
    Entity* _entity;
};
