//
//  EntityTopDownPhysicsController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "EntityPhysicsController.hpp"

#include <vector>

class Entity;
class TimeTracker;
struct Rektangle;

class EntityTopDownPhysicsController : public EntityPhysicsController
{
    DECL_EntityPhysicsController_create;
    
public:
    EntityTopDownPhysicsController(Entity* e) : EntityPhysicsController(e) {}
    virtual ~EntityTopDownPhysicsController() {}
    
    virtual void updatePoseFromBody();
    virtual void updateBodyFromPose();
    
    void processPhysics(TimeTracker* tt);
    void processCollisions(TimeTracker* tt, std::vector<Entity*>& entities);
    void enforceBounds(Rektangle& bounds);
};
