//
//  Entity.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 8/3/15.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "Entity.hpp"

#include "EntityLayout.hpp"
#include "EntityController.hpp"
#include "EntityNetworkController.hpp"
#include "EntityPhysicsController.hpp"
#include "EntityRenderController.hpp"
#include "MathUtil.hpp"
#include "EntityManager.hpp"
#include "Macros.hpp"
#include "Rektangle.hpp"
#include "OverlapTester.hpp"

Entity::Entity(EntityDef ed, EntityInstanceDef eid) :
_entityDef(ed),
_entityInstanceDef(eid),
_controller(ENTITY_MGR.createEntityController(ed, this)),
_networkController(ENTITY_MGR.createEntityNetworkController(ed, this)),
_physicsController(ENTITY_MGR.createEntityPhysicsController(ed, this)),
_renderController(ENTITY_MGR.createEntityRenderController(ed, this)),
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
    delete _renderController;
}

void Entity::update()
{
    if (_isRequestingDeletion)
    {
        return;
    }
    
    _physicsController->updatePoseFromBody();
    
    if (isDynamic())
    {
        ++_state._stateTime;
    }
    
    _controller->update();
}

void Entity::message(uint16_t message)
{
    if (_isRequestingDeletion)
    {
        return;
    }
    
    _controller->onMessage(message);
}

EntityDef& Entity::entityDef()
{
    return _entityDef;
}

uint16_t Entity::stateTime()
{
    return _state._stateTime;
}

Vector2& Entity::position()
{
    return _pose._position;
}

Vector2& Entity::velocity()
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

float Entity::angle()
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
    if (!isServer())
    {
        return;
    }
    
    _isRequestingDeletion = true;
}

bool Entity::isRequestingDeletion()
{
    return _isRequestingDeletion;
}

bool Entity::isXFlipped()
{
    return _pose._isXFlipped;
}

bool Entity::isBody()
{
    return _entityDef._bodyFlags > 0;
}

bool Entity::isLayer()
{
    return !isBody();
}

bool Entity::isStatic()
{
    return IS_BIT_SET(_entityDef._bodyFlags, BODF_STATIC);
}

bool Entity::isDynamic()
{
    return IS_BIT_SET(_entityDef._bodyFlags, BODF_DYNAMIC);
}

bool Entity::isPlayer()
{
    return IS_BIT_SET(_entityDef._bodyFlags, BODF_PLAYER);
}

bool Entity::isFixedRotation()
{
    return IS_BIT_SET(_entityDef._bodyFlags, BODF_FIXED_ROTATION);
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

bool Entity::isServer()
{
    return _entityInstanceDef._isServer;
}
