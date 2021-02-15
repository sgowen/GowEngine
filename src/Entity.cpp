//
//  Entity.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 8/3/15.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "Entity.hpp"

#include "EntityLayoutManager.hpp"
#include "EntityController.hpp"
#include "EntityNetworkController.hpp"
#include "EntityPhysicsController.hpp"
#include "MathUtil.hpp"
#include "EntityManager.hpp"
#include "Macros.hpp"
#include "Rektangle.hpp"
#include "OverlapTester.hpp"

Entity::Entity(EntityDef ed, EntityInstanceDef eid, bool isServer) :
_entityDef(ed),
_entityInstanceDef(eid),
_controller(ENTITY_MGR.createEntityController(ed, this)),
_physicsController(ENTITY_MGR.createEntityPhysicsController(ed, this)),
_networkController(ENTITY_MGR.createEntityNetworkController(ed, this, isServer)),
_pose(eid._x, eid._y),
_poseCache(_pose),
_state(),
_stateCache(_state),
_isRequestingDeletion(false)
{
    // Empty
}

Entity::~Entity()
{
    delete _controller;
    delete _networkController;
    delete _physicsController;
}

void Entity::update()
{
    if (_isRequestingDeletion)
    {
        return;
    }
    
    _physicsController->updatePoseFromBody();
    
    if (IS_BIT_SET(_entityDef._bodyFlags, BODF_DYNAMIC))
    {
        ++_state._stateTime;
    }
    
    _controller->update();
}

void Entity::message(uint16_t message, void* data)
{
    if (_isRequestingDeletion)
    {
        return;
    }
    
    _controller->onMessage(message, data);
}

EntityDef& Entity::entityDef()
{
    return _entityDef;
}

EntityController* Entity::controller()
{
    return _controller;
}

EntityPhysicsController* Entity::physicsController()
{
    return _physicsController;
}

EntityNetworkController* Entity::networkController()
{
    return _networkController;
}

uint16_t Entity::stateTime()
{
    return _state._stateTime;
}

void Entity::setPosition(Vector2 position)
{
    setPosition(position._x, position._y);
}

void Entity::setPosition(float x, float y)
{
    _pose._position.set(x, y);
    
    _physicsController->updateBodyFromPose();
}

const Vector2& Entity::getPosition()
{
    return _pose._position;
}

void Entity::setVelocity(Vector2 velocity)
{
    setVelocity(velocity._x, velocity._y);
}

void Entity::setVelocity(float x, float y)
{
    _pose._velocity.set(x, y);
    
    _physicsController->updateBodyFromPose();
}

const Vector2& Entity::getVelocity()
{
    return _pose._velocity;
}

float Entity::width()
{
    return _entityDef._width;
}

float Entity::height()
{
    return _entityDef._height;
}

void Entity::setAngle(float angle)
{
    _pose._angle = angle;
}

float Entity::getAngle()
{
    return _pose._angle;
}

const uint32_t Entity::getID()
{
    return _entityInstanceDef._ID;
}

bool Entity::isGrounded()
{
    return _pose._numGroundContacts > 0;
}

void Entity::requestDeletion()
{
    _isRequestingDeletion = true;
}

bool Entity::isRequestingDeletion()
{
    return _isRequestingDeletion;
}

bool Entity::isFacingLeft()
{
    return _pose._isFacingLeft;
}

std::string Entity::getTextureMapping()
{
    return getTextureMapping(_state._state);
}

std::string Entity::getTextureMapping(uint8_t state)
{
    auto q = _entityDef._textureMappings.find(state);
    
    if (q != _entityDef._textureMappings.end())
    {
        return q->second;
    }
    
    return _controller->getTextureMapping();
}

int Entity::getSoundMapping(int state)
{
    auto q1 = _entityDef._soundMappings.find(state);
    if (q1 != _entityDef._soundMappings.end())
    {
        return q1->second;
    }
    
    auto q2 = _entityDef._soundRandomMappings.find(state);
    if (q2 != _entityDef._soundRandomMappings.end())
    {
        std::vector<int> soundCollection = q2->second;
        
        int index = rand() % soundCollection.size();
        return soundCollection[index];
    }
    
    // No sound for this state
    return 0;
}

Entity::Pose& Entity::pose()
{
    return _pose;
}

Entity::Pose& Entity::poseCache()
{
    return _poseCache;
}

Entity::State& Entity::state()
{
    return _state;
}

Entity::State& Entity::stateCache()
{
    return _stateCache;
}
