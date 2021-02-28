//
//  EntityPhysicsController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "RTTI.hpp"
#include "Macros.hpp"

class Entity;

class EntityPhysicsController
{
    DECL_RTTI_NOPARENT;
    DECL_EntityController_create(EntityPhysicsController);
    
public:
    EntityPhysicsController(Entity* e);
    virtual ~EntityPhysicsController() {}
    
    virtual void updatePoseFromBody() {}
    virtual void updateBodyFromPose() {}
    
protected:
    Entity* _entity;
};
