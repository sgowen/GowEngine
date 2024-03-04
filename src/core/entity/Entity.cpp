//
//  Entity.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 8/3/15.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

Entity::Entity(EntityDef ed, EntityInstanceDef eid) :
_entityDef(ed),
_entityInstanceDef(eid),
_controller(nullptr),
_aiController(nullptr),
_inputController(nullptr),
_networkController(nullptr),
_physicsController(nullptr),
_renderController(nullptr),
_pose(eid._x + ed._width / 2.0f, eid._y + ed._height / 2.0f),
_state(),
_playerInfo(),
_exileStateTime(0),
_width(ed._width),
_height(ed._height),
_angle(0),
_world(nullptr)
{
    _controller = ENTITY_MGR.createEntityController(ed, this);
    _aiController = ENTITY_MGR.createEntityAIController(ed, this);
    _inputController = ENTITY_MGR.createEntityInputController(ed, this);
    _networkController = ENTITY_MGR.createEntityNetworkController(ed, this);
    _renderController = ENTITY_MGR.createEntityRenderController(ed, this);
}

Entity::~Entity()
{
    delete _controller;
    delete _inputController;
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
    
    if (isStateSensitive())
    {
        ++_state._stateTime;
    }
}

void Entity::runAI()
{
    if (isExiled())
    {
        return;
    }
    
    EntityAIController* eic = aiController();
    assert(eic != nullptr);
    
    uint16_t inputState = 0;
    eic->runAI(inputState);
    
    processInput(inputState);
}

void Entity::processEvent(uint16_t& inputState, CursorEvent* e)
{
    if (isExiled())
    {
        return;
    }
    
    EntityInputController* eic = inputController();
    assert(eic != nullptr);
    
    eic->processEvent(inputState, e);
}

void Entity::processEvent(uint16_t& inputState, GamepadEvent* e)
{
    if (isExiled())
    {
        return;
    }
    
    EntityInputController* eic = inputController();
    assert(eic != nullptr);
    
    eic->processEvent(inputState, e);
}

void Entity::processEvent(uint16_t& inputState, KeyboardEvent* e)
{
    if (isExiled())
    {
        return;
    }
    
    EntityInputController* eic = inputController();
    assert(eic != nullptr);
    
    eic->processEvent(inputState, e);
}

void Entity::processInput(uint16_t inputState)
{
    if (isExiled())
    {
        return;
    }
    
    EntityController* ec = controller();
    assert(ec != nullptr);
    
    ec->processInput(inputState);
    _state._lastProcessedInputState = inputState;
    
    if (_physicsController != nullptr)
    {
        _physicsController->updateBodyFromPose();
    }
}

void Entity::update(uint32_t numMovesProcessed)
{
    if (isExiled())
    {
        return;
    }
    
    if (position()._y < 0)
    {
        exile();
        return;
    }
    
    _controller->onUpdate(numMovesProcessed);
}

void Entity::message(uint16_t message, Entity* fromEntity)
{
    if (isExiled())
    {
        return;
    }
    
    _controller->onMessage(message, fromEntity);
}

EntityDef& Entity::entityDef()
{
    return _entityDef;
}

uint16_t Entity::inputStateFlag(std::string key)
{
    auto it = std::find(_entityDef._inputStateFlags.begin(), _entityDef._inputStateFlags.end(), key);
    assert(it != _entityDef._inputStateFlags.end());

    uint16_t index = std::distance(_entityDef._inputStateFlags.begin(), it);
    uint16_t ret = 1 << index;
    
    return ret;
}

uint8_t Entity::state(std::string key)
{
    auto it = std::find(_entityDef._states.begin(), _entityDef._states.end(), key);
    assert(it != _entityDef._states.end());

    uint8_t ret = std::distance(_entityDef._states.begin(), it);
    
    return ret;
}

std::string Entity::state(uint8_t state)
{
    assert(state < _entityDef._states.size());
    
    return _entityDef._states[state];
}

uint8_t Entity::stateFlag(std::string key)
{
    auto it = std::find(_entityDef._stateFlags.begin(), _entityDef._stateFlags.end(), key);
    assert(it != _entityDef._stateFlags.end());

    uint16_t index = std::distance(_entityDef._stateFlags.begin(), it);
    uint16_t ret = 1 << index;
    
    return ret;
}

Config& Entity::data()
{
    return _entityDef._data;
}

NetworkData& Entity::networkData()
{
    return _entityDef._networkData;
}

NetworkDataField& Entity::networkDataField(std::string name)
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

bool Entity::isStateSensitive()
{
    return IS_BIT_SET(_entityDef._bodyFlags, BODF_STATE_SENSITIVE);
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

Entity::PlayerInfo& Entity::playerInfo()
{
    return _playerInfo;
}

void Entity::exile()
{
    SET_BIT(_state._stateFlags, STTF_EXILED, true);
}

bool Entity::isExiled()
{
    return IS_BIT_SET(_state._stateFlags, STTF_EXILED);
}

bool Entity::isRequestingDeletion()
{
    return _exileStateTime >= 60;
}

uint16_t Entity::lastProcessedInputState()
{
    return _state._lastProcessedInputState;
}

void Entity::setWorld(World* w)
{
    _world = w;
}

World* Entity::world()
{
    return _world;
}
