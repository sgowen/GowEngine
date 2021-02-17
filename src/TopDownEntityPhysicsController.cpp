//
//  TopDownEntityPhysicsController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "TopDownEntityPhysicsController.hpp"

#include "Entity.hpp"
#include "TimeTracker.hpp"
#include "Rektangle.hpp"
#include "OverlapTester.hpp"
#include "EntityController.hpp"
#include "MathUtil.hpp"

IMPL_RTTI(TopDownEntityPhysicsController, EntityPhysicsController)
IMPL_EntityController_create(TopDownEntityPhysicsController, EntityPhysicsController)

void TopDownEntityPhysicsController::updatePoseFromBody()
{
    // TODO
}

void TopDownEntityPhysicsController::updateBodyFromPose()
{
    // TODO
}

void TopDownEntityPhysicsController::processPhysics(TimeTracker* tt)
{
    Vector2& vel = _entity->pose()._velocity;
    _entity->pose()._position += vel * tt->_frameRate;
    
    vel *= 0.9f;
    sanitizeCloseToZeroVector(vel._x, vel._y, 0.01f);
}

void TopDownEntityPhysicsController::processCollisions(std::vector<Entity*>& entities)
{
    float x = _entity->getPosition()._x;
    float y = _entity->getPosition()._y;
    float w = _entity->width();
    float h = _entity->height();
    Rektangle bounds(x - w / 2, y - h / 2, w, h);
    
    for (Entity* e : entities)
    {
        if (_entity == e)
        {
            continue;
        }
        
        float x = e->getPosition()._x;
        float y = e->getPosition()._y;
        float w = e->width();
        float h = e->height();
        Rektangle boundsToTest(x - w / 2, y - h / 2, w, h);
        
        if (OverlapTester::doRektanglesOverlap(bounds, boundsToTest))
        {
            if (bounds.right() >= boundsToTest.left() ||
                bounds.left() <= boundsToTest.right() ||
                bounds.top() >= boundsToTest.bottom() ||
                bounds.bottom() <= boundsToTest.top())
            {
//                Vector2 vel = _entity->getVelocity();
//                vel *= tt->_frameRate;
//                Vector2 pos = _entity->getPosition();
//                pos -= vel;
//                _entity->setPosition(pos);
            }
            
            onCollision(e);
            static_cast<TopDownEntityPhysicsController*>(e->physicsController())->onCollision(_entity);
            break;
        }
    }
}

void TopDownEntityPhysicsController::enforceBounds(Rektangle& bounds)
{
    float x = _entity->getPosition()._x;
    float y = _entity->getPosition()._y;
    
    if (x > bounds.right())
    {
        _entity->setPosition(bounds.right(), y);
    }
    else if (x < bounds.left())
    {
        _entity->setPosition(bounds.left(), y);
    }
    
    x = _entity->getPosition()._x;
    if (y > bounds.top())
    {
        _entity->setPosition(x, bounds.top());
    }
    else if (y < bounds.bottom())
    {
        _entity->setPosition(x, bounds.bottom());
    }
}
