//
//  Entity.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 8/3/15.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

Entity::Entity(EntityDef ed, EntityInstanceDef eid) :
_entityDef(ed),
_entityInstanceDef(eid),
_controller(ENTITY_MGR.createEntityController(ed, this)),
_networkController(ENTITY_MGR.createEntityNetworkController(ed, this)),
_physicsController(ENTITY_MGR.createEntityPhysicsController(ed, this)),
_renderController(ENTITY_MGR.createEntityRenderController(ed, this)),
_pose(eid._x + ed._width / 2.0f, eid._y + ed._height / 2.0f, ed._width, ed._height),
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
    
    _controller->onUpdate();
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

Config& Entity::metadata()
{
    return _entityDef._metadata;
}

NetworkData& Entity::data()
{
    return _entityDef._networkData;
}

NetworkDataField& Entity::dataField(std::string name)
{
    NetworkDataField* ret = nullptr;
    NetworkData& nd = _entityDef._networkData;
    for (NetworkDataGroup& ndg : nd._data)
    {
        for (NetworkDataField& ndf : ndg._data)
        {
            if (ndf._name == name)
            {
                ret = &ndf;
            }
        }
    }
    
    assert(ret != nullptr);
    
    return *ret;
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
    return _pose._width;
}

float Entity::height()
{
    return _pose._height;
}

void Entity::resetWidth()
{
    _pose._width = _entityDef._width;
}

void Entity::resetHeight()
{
    _pose._height = _entityDef._height;
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

bool Entity::isDynamicSize()
{
    return IS_BIT_SET(_entityDef._bodyFlags, BODF_DYNAMIC_SIZE);
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
