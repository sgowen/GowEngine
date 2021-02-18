//
//  TopDownPhysicsController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "TopDownPhysicsController.hpp"

#include "Entity.hpp"
#include "TimeTracker.hpp"
#include "Rektangle.hpp"
#include "OverlapTester.hpp"
#include "EntityController.hpp"
#include "MathUtil.hpp"

IMPL_RTTI(TopDownPhysicsController, EntityPhysicsController)
IMPL_EntityController_create(TopDownPhysicsController, EntityPhysicsController)

TopDownPhysicsController::TopDownPhysicsController(Entity* e) : EntityPhysicsController(e),
_boundingBox(NULL),
_bounds(NULL)
{
    // Empty
}

TopDownPhysicsController::~TopDownPhysicsController()
{
    if (_boundingBox != NULL)
    {
        delete _boundingBox;
    }
    
    if (_bounds != NULL)
    {
        delete _bounds;
    }
}

void TopDownPhysicsController::updatePoseFromBody()
{
    if (_boundingBox == NULL ||
        _bounds == NULL)
    {
        return;
    }
    
    _entity->pose()._position.set(_boundingBox->centerX(), _boundingBox->centerY());
}

void TopDownPhysicsController::updateBodyFromPose()
{
    if (_boundingBox == NULL ||
        _bounds == NULL)
    {
        return;
    }
    
    float x = _entity->position()._x;
    float y = _entity->position()._y;
    float w = _entity->width();
    float h = _entity->height();
    
    // TODO, hit box generation needs to come from fixture data entities.json
    _boundingBox->_lowerLeft.set(x - w / 2, y - h / 2);
    _boundingBox->_width = w;
    _boundingBox->_height = h;
    
    updateBounds();
}

void TopDownPhysicsController::initPhysics()
{
    if (_entity->entityDef()._fixtures.size() > 0)
    {
        _boundingBox = new Rektangle(-1, -1, 1, 1);
        _bounds = new Bounds();
        updateBodyFromPose();
    }
}

void TopDownPhysicsController::processPhysics(TimeTracker* tt)
{
    Vector2& vel = _entity->pose()._velocity;
    _entity->pose()._position += vel * tt->_frameRate;
    
    vel *= 0.9f;
    sanitizeCloseToZeroVector(vel._x, vel._y, 0.01f);
    
    updateBodyFromPose();
}

void TopDownPhysicsController::processCollisions(std::vector<Entity*>& entities)
{
    if (_boundingBox == NULL ||
        _bounds == NULL)
    {
        return;
    }
    
    for (Entity* e : entities)
    {
        if (_entity == e)
        {
            continue;
        }
        
        Rektangle* target = e->physicsController<TopDownPhysicsController>()->boundingBox();
        if (target == NULL)
        {
            continue;
        }
        
        static float fudgeFactor = 0.01f;
        bool isCollision = false;
        
        if (OverlapTester::doLineAndRektangleOverlap(_bounds->_left, *target))
        {
            _boundingBox->_lowerLeft.add(target->right() - _boundingBox->left() + fudgeFactor, 0);
            updateBounds();
            isCollision = true;
        }
        
        if (OverlapTester::doLineAndRektangleOverlap(_bounds->_right, *target))
        {
            _boundingBox->_lowerLeft.sub(_boundingBox->right() - target->left() - fudgeFactor, 0);
            updateBounds();
            isCollision = true;
        }
        
        if (OverlapTester::doLineAndRektangleOverlap(_bounds->_bottom, *target))
        {
            _boundingBox->_lowerLeft.add(0, target->top() - _boundingBox->bottom() + fudgeFactor);
            updateBounds();
            isCollision = true;
        }
        
        if (OverlapTester::doLineAndRektangleOverlap(_bounds->_top, *target))
        {
            _boundingBox->_lowerLeft.sub(0, _boundingBox->top() - target->bottom() - fudgeFactor);
            updateBounds();
            isCollision = true;
        }
        
        if (isCollision)
        {
            onCollision(e);
            e->physicsController<TopDownPhysicsController>()->onCollision(_entity);
        }
    }
}

void TopDownPhysicsController::enforceBounds(Rektangle& bounds)
{
    if (_boundingBox == NULL ||
        _bounds == NULL)
    {
        return;
    }
    
    if (_boundingBox->right() > bounds.right())
    {
        _boundingBox->_lowerLeft.sub(_boundingBox->right() - bounds.right(), 0);
    }
    else if (_boundingBox->left() < bounds.left())
    {
        _boundingBox->_lowerLeft.add(bounds.left() - _boundingBox->left(), 0);
    }
    
    if (_boundingBox->top() > bounds.top())
    {
        _boundingBox->_lowerLeft.sub(0, _boundingBox->top() - bounds.top());
    }
    else if (_boundingBox->bottom() < bounds.bottom())
    {
        _boundingBox->_lowerLeft.add(0, bounds.bottom() - _boundingBox->bottom());
    }
    
    updateBounds();
}

Rektangle* TopDownPhysicsController::boundingBox()
{
    return _boundingBox;
}

void TopDownPhysicsController::updateBounds()
{
    _bounds->_left._origin.set(_boundingBox->left(), _boundingBox->bottom());
    _bounds->_left._end.set(_boundingBox->left(), _boundingBox->top());
    _bounds->_right._origin.set(_boundingBox->right(), _boundingBox->bottom());
    _bounds->_right._end.set(_boundingBox->right(), _boundingBox->top());
    _bounds->_bottom._origin.set(_boundingBox->left(), _boundingBox->bottom());
    _bounds->_bottom._end.set(_boundingBox->right(), _boundingBox->bottom());
    _bounds->_top._origin.set(_boundingBox->left(), _boundingBox->top());
    _bounds->_top._end.set(_boundingBox->right(), _boundingBox->top());
}
