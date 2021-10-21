//
//  NosPhysicsController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI(NosPhysicsController, EntityPhysicsController)

NosPhysicsController::NosPhysicsController(Entity* e) : EntityPhysicsController(e),
_velocity(e->velocity()),
_position(e->position()),
_groundSensor(nullptr),
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

void NosPhysicsController::step(float gravity, float friction, float deltaTime)
{
    if (_entity->isGrounded())
    {
        if (_velocity._x > 0)
        {
            friction = -friction;
        }
        else if (_velocity._x == 0)
        {
            friction = 0;
        }
    }
    else
    {
        friction = 0;
    }
    
    _velocity.add(friction, gravity * deltaTime);
    _position.add(_velocity._x * deltaTime, _velocity._y * deltaTime);
    _numGroundContacts = 0;
}

void NosPhysicsController::processCollisions(std::vector<Entity*>& entities)
{
//    static float fudgeFactor = 0.001f;
    
    for (Entity* e : entities)
    {
        if (_entity == e)
        {
            // Don't collide with myself
            continue;
        }
        
        NosPhysicsController* epc = e->physicsController<NosPhysicsController>();
        
        for (Bounds& myBounds : _bounds)
        {
            Rektangle& myBoundingBox = myBounds._boundingBox;
            bool isGroundSensor = _groundSensor == &myBounds;
            
            for (Bounds& yourBounds : epc->_bounds)
            {
                Rektangle& yourBoundingBox = yourBounds._boundingBox;
                if (OverlapTester::doRektanglesOverlap(myBoundingBox, yourBoundingBox))
                {
                    if (isGroundSensor)
                    {
                        if (OverlapTester::doLineAndRektangleOverlap(myBounds._bottom, yourBoundingBox))
                        {
                            ++_numGroundContacts;
                        }
                    }
                    else
                    {
                        if (OverlapTester::doLineAndRektangleOverlap(myBounds._left, yourBoundingBox))
                        {
                            _position.add(yourBoundingBox.right() - myBoundingBox.left(), 0);
                        }
                        
                        if (OverlapTester::doLineAndRektangleOverlap(myBounds._right, yourBoundingBox))
                        {
                            _position.sub(myBoundingBox.right() - yourBoundingBox.right(), 0);
                        }
                        
                        if (OverlapTester::doLineAndRektangleOverlap(myBounds._bottom, yourBoundingBox))
                        {
                            _position.add(0, yourBoundingBox.top() - myBoundingBox.bottom());
                        }
                        
                        if (OverlapTester::doLineAndRektangleOverlap(myBounds._top, yourBoundingBox))
                        {
                            _position.sub(0, myBoundingBox.top() - yourBoundingBox.bottom());
                        }
                    }
                }
            }
            
            myBounds.updateForPosition(_position);
        }
    }
}

std::vector<Bounds>& NosPhysicsController::bounds()
{
    return _bounds;
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
        
        _bounds.emplace_back(centerX, centerY, halfWidth, halfHeight);
        Bounds& b = _bounds.back();
        b.updateForPosition(_entity->position());
        if (IS_BIT_SET(fd._flags, FIXF_GROUND_CONTACT))
        {
            _groundSensor = &_bounds.back();
        }
    }
}

void NosPhysicsController::destroyFixtures()
{
    _bounds.clear();
    
    _groundSensor = nullptr;
}
