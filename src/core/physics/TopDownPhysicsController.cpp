//
//  TopDownPhysicsController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI(TopDownPhysicsController, EntityPhysicsController)
IMPL_EntityController_create(TopDownPhysicsController, EntityPhysicsController)

TopDownPhysicsController::TopDownPhysicsController(Entity* e) : EntityPhysicsController(e),
_boundingBox(nullptr),
_bounds(nullptr)
{
    // Empty
}

TopDownPhysicsController::~TopDownPhysicsController()
{
    if (_boundingBox != nullptr)
    {
        delete _boundingBox;
    }
    
    if (_bounds != nullptr)
    {
        delete _bounds;
    }
}

void TopDownPhysicsController::updatePoseFromBody()
{
    if (_boundingBox == nullptr ||
        _bounds == nullptr)
    {
        return;
    }
    
    _entity->pose()._position.set(_boundingBox->centerX(), _boundingBox->centerY());
}

void TopDownPhysicsController::updateBodyFromPose()
{
    if (_boundingBox == nullptr ||
        _bounds == nullptr)
    {
        return;
    }
    
    float x = _entity->position()._x;
    float y = _entity->position()._y;
    float w = _entity->width();
    float h = _entity->height();
    
    // TODO, hit box generation needs to come from fixture data entities.json
    
    bool useInset = false;//_entity->entityDef()._fixtures[0]._flags == 1;
    _boundingBox->_lowerLeft.set(x - w * (useInset ? 0.4f : 0.5f), y - h * (useInset ? 0.4f : 0.5f));
    _boundingBox->_width = w * (useInset ? 0.8f : 1.0f);
    _boundingBox->_height = h * (useInset ? 0.8f : 1.0f);
    
    updateBounds();
}

void TopDownPhysicsController::initPhysics()
{
    if (_entity->entityDef()._fixtures.empty())
    {
        return;
    }
    
    _boundingBox = new Rektangle(-1, -1, 1, 1);
    _bounds = new Bounds();
    updateBodyFromPose();
}

void TopDownPhysicsController::processPhysics(TimeTracker* tt)
{
    Vector2& vel = _entity->pose()._velocity;
    _entity->pose()._position += vel * static_cast<float>(tt->_frameRate);
    
    vel *= 0.9f;
    sanitizeCloseToZeroVector(vel._x, vel._y, 0.01f);
    
    updateBodyFromPose();
}

void TopDownPhysicsController::processCollisions(std::vector<Entity*>& entities)
{
    if (_boundingBox == nullptr ||
        _bounds == nullptr)
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
        if (target == nullptr)
        {
            continue;
        }
        
        static float fudgeFactor = 0.001f;
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
            _entity->controller()->onCollision(e);
            e->controller()->onCollision(_entity);
        }
    }
}

void TopDownPhysicsController::push(int dir, float force)
{    
    switch (dir)
    {
        case EDIR_UP:
            _entity->velocity()._y = force;
            break;
        case EDIR_DOWN:
            _entity->velocity()._y = -force;
            break;
        case EDIR_LEFT:
            _entity->velocity()._x = -force;
            break;
        case EDIR_RIGHT:
            _entity->velocity()._x = force;
            break;
        default:
            break;
    }
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
