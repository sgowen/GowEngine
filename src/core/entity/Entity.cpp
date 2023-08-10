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
_physicsController(nullptr),
_renderController(ENTITY_MGR.createEntityRenderController(ed, this)),
_pose(eid._x + ed._width / 2.0f, eid._y + ed._height / 2.0f),
_state(),
_exileStateTime(0),
_width(ed._width),
_height(ed._height),
_angle(0),
_world(nullptr)
{
    // Empty
}

Entity::~Entity()
{
    delete _controller;
    delete _networkController;
    if (_physicsController != nullptr)
    {
        delete _physicsController;
    }
    delete _renderController;
}

void Entity::beginFrame()
{
    if (isExiled())
    {
        ++_exileStateTime;
        return;
    }
    
    ++_state._stateTime;
}

void Entity::processInput(uint16_t inputState)
{
    _state._inputState = inputState;
    
    if (isExiled())
    {
        return;
    }
    
    EntityController* ec = controller();
    assert(ec != nullptr);
    ec->processInput(inputState);
    
    if (_physicsController != nullptr)
    {
        _physicsController->updateBodyFromPose();
    }
}

void Entity::update()
{
    if (isExiled())
    {
        return;
    }
    
    if (_physicsController != nullptr)
    {
        _physicsController->updatePoseFromBody();
    }
    
    _controller->onUpdate();
}

void Entity::message(uint16_t message)
{
    if (isExiled())
    {
        return;
    }
    
    _controller->onMessage(message);
}

EntityDef& Entity::entityDef()
{
    return _entityDef;
}

Config& Entity::data()
{
    return _entityDef._data;
}

NetworkData& Entity::networkData()
{
    return _entityDef._networkData;
}

NetworkDataField& Entity::dataField(std::string name)
{
    NetworkDataField* ret = nullptr;
    NetworkData& nd = networkData();
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

bool Entity::isGrounded()
{
    return _pose._numGroundContacts > 0;
}

bool Entity::isXFlipped()
{
    return _pose._isXFlipped;
}

float Entity::width()
{
    return _width;
}

float Entity::height()
{
    return _height;
}

float Entity::angle()
{
    return _angle;
}

const uint32_t Entity::getID()
{
    return _entityInstanceDef._ID;
}

bool Entity::isLayer()
{
    return _entityDef._fixtures.empty();
}

bool Entity::isStatic()
{
    return !IS_BIT_SET(_entityDef._bodyFlags, BODF_DYNAMIC);
}

bool Entity::isDynamic()
{
    return IS_BIT_SET(_entityDef._bodyFlags, BODF_DYNAMIC);
}

bool Entity::isPlayer()
{
    return IS_BIT_SET(_entityDef._bodyFlags, BODF_PLAYER);
}

bool Entity::isServer()
{
    return _entityInstanceDef._isServer;
}

Entity::Pose& Entity::pose()
{
    return _pose;
}

Entity::State& Entity::state()
{
    return _state;
}

void Entity::exile()
{
    _state._stateFlags = STTF_EXILED;
}

bool Entity::isExiled()
{
    return _state._stateFlags == STTF_EXILED;
}

bool Entity::isRequestingDeletion()
{
    return _exileStateTime >= 60;
}

uint16_t Entity::lastInputState()
{
    return _state._inputState;
}

void Entity::setWorld(World* w)
{
    _world = w;
}

World* Entity::world()
{
    return _world;
}
