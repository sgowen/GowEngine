//
//  NosPhysicsController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI(NosPhysicsController, EntityPhysicsController)

NosPhysicsController::NosPhysicsController(Entity* e, float gravity) : EntityPhysicsController(e),
_velocity(e->velocity()),
_velocityCache(_velocity),
_position(e->position()),
_positionCache(_position),
_gravity(gravity),
_numGroundContacts(0),
_isBodyFacingLeft(false)
{
    createFixtures();
}

NosPhysicsController::~NosPhysicsController()
{
    destroyFixtures();
}

Vector2 NosPhysicsController::velocity()
{
    return _velocity;
}

void NosPhysicsController::setVelocity(Vector2 v)
{
    _velocity.set(v);
}

void NosPhysicsController::updatePoseFromBody()
{
    _entity->velocity().set(_velocity._x, _velocity._y);
    _entity->position().set(_position._x, _position._y);
    _entity->pose()._numGroundContacts = _numGroundContacts;
}

void NosPhysicsController::updateBodyFromPose()
{
    _velocity.set(_entity->velocity());
    _position.set(_entity->position());
    _numGroundContacts = _entity->pose()._numGroundContacts;
    
    if (_isBodyFacingLeft != _entity->isXFlipped())
    {
        destroyFixtures();
        createFixtures();
    }
    
    for (Bounds& b : _bounds)
    {
        b.updateForPosition(_position);
    }
}

void NosPhysicsController::step(float deltaTime)
{
    if (_entity->isExiled())
    {
        return;
    }
    
    _velocity.add(0, _gravity * deltaTime);
    _tolerance = fabsf(_velocity._y * deltaTime * 2);
    _position.add(_velocity._x * deltaTime, _velocity._y * deltaTime);
    for (Bounds& b : _bounds)
    {
        b.updateForPosition(_position);
    }
    
    _numGroundContacts = 0;
    
    if (ENGINE_CFG.physicsLoggingEnabled())
    {
        LOG("_velocity: %f, %f", _velocity._x, _velocity._y);
        LOG("_position: %f, %f", _position._x, _position._y);
    }
}

void NosPhysicsController::interpolate(float interpolation)
{
    _velocityCache = _velocity;
    _positionCache = _position;
    
    _velocity.add(0, _gravity * interpolation);
    _position.add(_velocity._x * interpolation, _velocity._y * interpolation);
}

void NosPhysicsController::endInterpolation()
{
    _velocity = _velocityCache;
    _position = _positionCache;
}

void NosPhysicsController::processCollisions(std::vector<Entity*>& entities)
{
    if (_entity->isExiled())
    {
        return;
    }
    
    for (Entity* e : entities)
    {
        if (_entity == e)
        {
            // Don't collide with myself
            continue;
        }
        
        if (e->isExiled())
        {
            // Can't interact with something in exile
            continue;
        }
        
        NosPhysicsController* epc = e->physicsController<NosPhysicsController>();
        
        bool t = false;
        bool b = false;
        bool r = false;
        bool l = false;
        
        for (Bounds& myBounds : _bounds)
        {
            if (IS_BIT_SET(myBounds._flags, FIXF_GROUND_SENSOR))
            {
                continue;
            }
            
            Rektangle& myBoundingBox = myBounds._boundingBox;
            for (Bounds& yourBounds : epc->_bounds)
            {
                Rektangle& yourBoundingBox = yourBounds._boundingBox;
                uint32_t yourFlags = yourBounds._flags;
                if (OverlapTester::doRektanglesOverlap(myBoundingBox, yourBoundingBox))
                {
                    {
                        float mt = myBoundingBox.top();
                        float mb = myBoundingBox.bottom();
                        
                        float mr = myBoundingBox.right();
                        float ml = myBoundingBox.left();
                        
                        float yt = yourBoundingBox.top();
                        float yb = yourBoundingBox.bottom();
                        
                        float yr = yourBoundingBox.right();
                        float yl = yourBoundingBox.left();
                        
                        t = !IS_BIT_SET(yourFlags, FIXF_IGNORE_BOTTOM) &&
                        crossesBottomEdge(yb, mt, _tolerance) &&
                        mb < yb && (mr > yl || ml < yr);
                        
                        b = _velocity._y < 0 &&
                        crossesTopEdge(yt, mb, _tolerance) &&
                        mt > yt && (mr > yl || ml < yr);
                        
                        if (t)
                        {
                            _position.sub(0, mt - yb);
                            _velocity._y = 0;
                            myBounds.updateForPosition(_position);
                            if (ENGINE_CFG.physicsLoggingEnabled())
                            {
                                LOG("top");
                            }
                        }
                        else if (b)
                        {
                            _position.add(0, yt - mb);
                            _velocity._y = 0;
                            myBounds.updateForPosition(_position);
                            if (ENGINE_CFG.physicsLoggingEnabled())
                            {
                                LOG("bottom");
                            }
                        }
                    }
                    
                    {
                        float mt = myBoundingBox.top();
                        float mb = myBoundingBox.bottom();
                        
                        float mr = myBoundingBox.right();
                        float ml = myBoundingBox.left();
                        
                        float yt = yourBoundingBox.top();
                        float yb = yourBoundingBox.bottom();
                        
                        float yr = yourBoundingBox.right();
                        float yl = yourBoundingBox.left();
                        
                        bool i = isInside(yb, yt, mb, mt);
                        if (ENGINE_CFG.physicsLoggingEnabled())
                        {
                            LOG("isInside: %d", i);
                        }
                        
                        r = !IS_BIT_SET(yourFlags, FIXF_IGNORE_LEFT) &&
                        mr > yl && ml < yl && i;
                        
                        l = !IS_BIT_SET(yourFlags, FIXF_IGNORE_RIGHT) &&
                        ml < yr && mr > yr && i;
                        
                        if (r)
                        {
                            _position.sub(mr - yl, 0);
                            _velocity._x = 0;
                            myBounds.updateForPosition(_position);
                            if (ENGINE_CFG.physicsLoggingEnabled())
                            {
                                LOG("right");
                            }
                        }
                        else if (l)
                        {
                            _position.add(yr - ml, 0);
                            _velocity._x = 0;
                            myBounds.updateForPosition(_position);
                            if (ENGINE_CFG.physicsLoggingEnabled())
                            {
                                LOG("left");
                            }
                        }
                    }
                }
            }
        }
        
        for (Bounds& myBounds : _bounds)
        {
            if (!IS_BIT_SET(myBounds._flags, FIXF_GROUND_SENSOR))
            {
                continue;
            }
            
            myBounds.updateForPosition(_position);
            Rektangle& myBoundingBox = myBounds._boundingBox;
            for (Bounds& yourBounds : epc->_bounds)
            {
                Rektangle& yourBoundingBox = yourBounds._boundingBox;
                if (b &&
                    OverlapTester::doRektanglesOverlap(myBoundingBox, yourBoundingBox))
                {
                    ++_numGroundContacts;
                }
            }
        }
    }
}

std::vector<Bounds>& NosPhysicsController::bounds()
{
    return _bounds;
}

bool NosPhysicsController::crossesBottomEdge(float yourBottom, float myTop, float tolerance)
{
    return areFloatsPracticallyEqual(myTop, yourBottom) ? false : myTop > yourBottom && myTop < yourBottom + tolerance;
}

bool NosPhysicsController::crossesTopEdge(float yourTop, float myBottom, float tolerance)
{
    return areFloatsPracticallyEqual(myBottom, yourTop) ? false : myBottom < yourTop && myBottom > yourTop - tolerance;
}

bool NosPhysicsController::isInside(float yourBottom, float yourTop, float myBottom, float myTop)
{
    return myBottom < yourTop || (myTop > yourBottom && myTop < yourTop);
}

void NosPhysicsController::createFixtures()
{
    _isBodyFacingLeft = _entity->isXFlipped();
    
    float bodyWidth = _entity->width();
    float bodyHeight = _entity->height();
    
    std::vector<FixtureDef>& fixtures = _entity->entityDef()._fixtures;
    for (std::vector<FixtureDef>::iterator i = fixtures.begin(); i != fixtures.end(); ++i)
    {
        FixtureDef fd = *i;
        if (_isBodyFacingLeft)
        {
            fd._center._x = -fd._center._x;
        }
        
        float centerX = fd._center._x * bodyWidth;
        float centerY = fd._center._y * bodyHeight;
        float halfWidth = bodyWidth * fd._halfWidthFactor;
        float halfHeight = bodyHeight * fd._halfHeightFactor;
        
        _bounds.emplace_back(fd._flags, centerX, centerY, halfWidth, halfHeight);
        Bounds& b = _bounds.back();
        b.updateForPosition(_entity->position());
    }
}

void NosPhysicsController::destroyFixtures()
{
    _bounds.clear();
}
