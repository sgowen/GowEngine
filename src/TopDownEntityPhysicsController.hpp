//
//  TopDownEntityPhysicsController.hpp
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

class TopDownEntityPhysicsController : public EntityPhysicsController
{
    DECL_RTTI;
    DECL_EntityController_create(EntityPhysicsController);
    
public:
    TopDownEntityPhysicsController(Entity* e) : EntityPhysicsController(e) {}
    virtual ~TopDownEntityPhysicsController() {}
    
    virtual void updatePoseFromBody() override;
    virtual void updateBodyFromPose() override;
    
    void processPhysics(TimeTracker* tt);
    void processCollisions(std::vector<Entity*>& entities);
    void enforceBounds(Rektangle& bounds);
    
protected:
    virtual void onCollision(Entity* e) {}
};
