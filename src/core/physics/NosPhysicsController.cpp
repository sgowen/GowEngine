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
    _numGroundContacts = _entity->pose()._numGroundContacts;
    
    if (_isBodyFacingLeft != _entity->isXFlipped())
    {
        destroyFixtures();
        createFixtures();
    }
}

void NosPhysicsController::step(float gravity, float deltaTime)
{
    _velocity.add(0, gravity * deltaTime);
    _position.add(_velocity._x * deltaTime, _velocity._y * deltaTime);
    _numGroundContacts = 0;
}

void NosPhysicsController::processCollisions(std::vector<Entity*>& entities)
{
    for (Entity* e : entities)
    {
        if (_entity == e)
        {
            // Don't collide with yourself
            continue;
        }
        
        NosPhysicsController* epc = e->physicsController<NosPhysicsController>();
        
        for (Rektangle& myBoundingBox : _boundingBoxes)
        {
            bool isGroundSensor = _groundSensor == &myBoundingBox;
            
            for (Rektangle& yourBoundingBox : epc->_boundingBoxes)
            {
                
            }
        }
    }
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
        
        float wFactor = bodyWidth * fd._halfWidth;
        float hFactor = bodyHeight * fd._halfHeight;
        float x = fd._center._x - wFactor;
        float y = fd._center._y - hFactor;
        
        Rektangle r(x, y, wFactor * 2, hFactor * 2);
        
        _boundingBoxes.push_back(r);
        if (IS_BIT_SET(fd._flags, FIXF_GROUND_CONTACT))
        {
            _groundSensor = &_boundingBoxes.back();
        }
    }
}

void NosPhysicsController::destroyFixtures()
{
    _boundingBoxes.clear();
    
    _groundSensor = nullptr;
}
