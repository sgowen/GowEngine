//
//  TopDownPhysicsController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "EntityPhysicsController.hpp"
#include "Line.hpp"

#include <vector>

class Entity;
class TimeTracker;
struct Rektangle;

struct Bounds
{
    Line _left;
    Line _right;
    Line _bottom;
    Line _top;
    
    Bounds() :
    _left(0, 0, 0, 0),
    _right(0, 0, 0, 0),
    _bottom(0, 0, 0, 0),
    _top(0, 0, 0, 0)
    {
        // Empty
    }
};

class TopDownPhysicsController : public EntityPhysicsController
{
    DECL_RTTI;
    DECL_EntityController_create(EntityPhysicsController);
    
public:
    TopDownPhysicsController(Entity* e);
    virtual ~TopDownPhysicsController();
    
    virtual void updatePoseFromBody() override;
    virtual void updateBodyFromPose() override;
    
    void initPhysics();
    void processPhysics(TimeTracker* tt);
    void processCollisions(std::vector<Entity*>& entities);
    Rektangle* boundingBox();
    
protected:
    virtual void onCollision(Entity* e) {}
    
private:
    Rektangle* _boundingBox;
    Bounds* _bounds;
    
    void updateBounds();
};
