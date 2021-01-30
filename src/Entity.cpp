//
//  Entity.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 8/3/15.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "Entity.hpp"

#include "EntityController.hpp"
#include "EntityNetworkController.hpp"
#include "MathUtil.hpp"
#include <box2d/box2d.h>

#include "EntityMapper.hpp"
#include "Constants.hpp"
#include "Macros.hpp"

Entity::Entity(EntityDef entityDef) :
_entityDef(entityDef),
_controller(ENTITY_MAPPER.createEntityController(entityDef, this)),
_networkController(ENTITY_MAPPER.createEntityNetworkController(entityDef, this)),
_body(NULL),
_groundSensorFixture(NULL),
_pose(entityDef._x, entityDef._y),
_poseNetworkCache(_pose),
_ID(entityDef._ID),
_deadZoneY(-_entityDef._height / 2.0f),
_isRequestingDeletion(false),
_isBodyFacingLeft(false)
{
    // Empty
}

Entity::~Entity()
{
    delete _controller;
    delete _networkController;
}

void Entity::update()
{
    updatePoseFromBody();
    
    if (getPosition().y < _deadZoneY)
    {
        requestDeletion();
    }
    
    if (_entityDef._stateSensitive)
    {
        ++_state._stateTime;
    }
    
    _controller->update();
}

void Entity::selfProcessPhysics()
{
    _pose._velocity *= FRAME_RATE;
    _pose._position += _pose._velocity;
    
    // TODO, add controller method to check for collisions
}

bool Entity::shouldCollide(Entity *e, b2Fixture* fixtureA, b2Fixture* fixtureB)
{
    if (fixtureA == _groundSensorFixture)
    {
        return e->getEntityDef()._key != _entityDef._key;
    }
    
    return _controller->shouldCollide(e, fixtureA, fixtureB);
}

void Entity::handleBeginContact(Entity* e, b2Fixture* fixtureA, b2Fixture* fixtureB)
{
    if (fixtureA == _groundSensorFixture &&
        !fixtureB->IsSensor())
    {
        _pose._numGroundContacts = clamp(_pose._numGroundContacts + 1, 0, 15);
    }
    
    _controller->handleBeginContact(e, fixtureA, fixtureB);
}

void Entity::handleEndContact(Entity* e, b2Fixture* fixtureA, b2Fixture* fixtureB)
{
    if (fixtureA == _groundSensorFixture &&
        !fixtureB->IsSensor())
    {
        _pose._numGroundContacts = clamp(_pose._numGroundContacts - 1, 0, 15);
    }
    
    _controller->handleEndContact(e, fixtureA, fixtureB);
}

void Entity::initPhysics(b2World& world)
{
    assert(_body == NULL);
    
    b2BodyDef bodyDef;
    bodyDef.position.Set(_pose._position.x, _pose._position.y);
    bodyDef.type = IS_BIT_SET(_entityDef._bodyFlags, BODF_STATIC) ? b2_staticBody : b2_dynamicBody;
    bodyDef.fixedRotation = IS_BIT_SET(_entityDef._bodyFlags, BODF_FIXED_ROTATION);
    bodyDef.bullet = IS_BIT_SET(_entityDef._bodyFlags, BODF_BULLET);
    bodyDef.userData.pointer = (uintptr_t)this;
    _body = world.CreateBody(&bodyDef);
    
    createFixtures();
}

void Entity::deinitPhysics()
{
    assert(_body != NULL);
    
    destroyFixtures();
    
    b2World* world = _body->GetWorld();
    world->DestroyBody(_body);
    _body = NULL;
}

void Entity::updatePoseFromBody()
{
    if (_body == NULL)
    {
        return;
    }
    
    _pose._velocity = _body->GetLinearVelocity();
    _pose._position = _body->GetPosition();
    _pose._angle = _body->GetAngle();
}

void Entity::updateBodyFromPose()
{
    if (_body == NULL)
    {
        return;
    }
    
    _body->SetLinearVelocity(_pose._velocity);
    _body->SetTransform(_pose._position, _pose._angle);
    
    if (_isBodyFacingLeft != _pose._isFacingLeft)
    {
        destroyFixtures();
        _isBodyFacingLeft = _pose._isFacingLeft;
        createFixtures();
    }
}

EntityDef& Entity::getEntityDef()
{
    return _entityDef;
}

EntityController* Entity::getController()
{
    return _controller;
}

EntityNetworkController* Entity::getNetworkController()
{
    return _networkController;
}

uint16_t Entity::getStateTime()
{
    return _state._stateTime;
}

b2Body* Entity::getBody()
{
    return _body;
}

void Entity::setPosition(b2Vec2 position)
{
    _pose._position = position;
    
    updateBodyFromPose();
}

const b2Vec2& Entity::getPosition()
{
    return _pose._position;
}

void Entity::setVelocity(b2Vec2 velocity)
{
    _pose._velocity = velocity;
    
    updateBodyFromPose();
}

const b2Vec2& Entity::getVelocity()
{
    return _pose._velocity;
}

float Entity::getWidth()
{
    return _entityDef._width;
}

float Entity::getHeight()
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
    return _ID;
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

std::string& Entity::getTextureMapping()
{
    return getTextureMapping(_state._state);
}

std::string& Entity::getTextureMapping(uint8_t state)
{
    auto q = _entityDef._textureMappings.find(state);
    
    assert(q != _entityDef._textureMappings.end());
    
    return q->second;
}

int Entity::getSoundMapping(int state)
{
    auto q = _entityDef._soundMappings.find(state);
    
    if (q != _entityDef._soundMappings.end())
    {
        return q->second;
    }
    
    // No sound for this state
    return 0;
}

bool Entity::isFixedRotation() const
{
    return IS_BIT_SET(_entityDef._bodyFlags, BODF_FIXED_ROTATION);
}

Entity::Pose& Entity::pose()
{
    return _pose;
}

Entity::Pose& Entity::poseNetworkCache()
{
    return _poseNetworkCache;
}

Entity::State& Entity::state()
{
    return _state;
}

Entity::State& Entity::stateNetworkCache()
{
    return _stateNetworkCache;
}

void Entity::createFixtures()
{
    for (std::vector<FixtureDef>::iterator i = _entityDef._fixtures.begin(); i != _entityDef._fixtures.end(); ++i)
    {
        FixtureDef def = *i;
        if (_isBodyFacingLeft)
        {
            def._center.x = -def._center.x;
        }
        
        b2Shape* shape;
        b2PolygonShape polygonShape;
        b2CircleShape circleShape;
        if (IS_BIT_SET(def._flags, FIXF_CIRCLE))
        {
            circleShape.m_p.Set(def._center.x * _entityDef._width, def._center.y * _entityDef._height);
            circleShape.m_radius = def._vertices[0].x * _entityDef._width;
            
            shape = &circleShape;
        }
        else
        {
            if (IS_BIT_SET(def._flags, FIXF_BOX))
            {
                float wFactor = _entityDef._width * def._vertices[0].x;
                float hFactor = _entityDef._height * def._vertices[0].y;
                def._center.Set(def._center.x * _entityDef._width, def._center.y * _entityDef._height);
                
                polygonShape.SetAsBox(wFactor, hFactor, def._center, 0);
            }
            else
            {
                for (std::vector<b2Vec2>::iterator i = def._vertices.begin(); i != def._vertices.end(); ++i)
                {
                    b2Vec2& vertex = (*i);
                    vertex.Set(vertex.x * _entityDef._width, vertex.y * _entityDef._height);
                }
                
                int count = static_cast<int>(def._vertices.size());
                polygonShape.Set(&def._vertices[0], count);
            }
            shape = &polygonShape;
        }
        
        b2FixtureDef fixtureDef;
        fixtureDef.shape = shape;
        fixtureDef.isSensor = IS_BIT_SET(def._flags, FIXF_SENSOR);
        fixtureDef.density = def._density;
        fixtureDef.friction = def._friction;
        fixtureDef.restitution = def._restitution;
        fixtureDef.userData.pointer = (uintptr_t)this;
        b2Fixture* fixture = _body->CreateFixture(&fixtureDef);
        
        if (IS_BIT_SET(def._flags, FIXF_GROUND_CONTACT))
        {
            _groundSensorFixture = fixture;
        }
        
        _fixtures.push_back(fixture);
    }
    
    _controller->onFixturesCreated(_fixtures);
}

void Entity::destroyFixtures()
{
    for (std::vector<b2Fixture*>::iterator i = _fixtures.begin(); i != _fixtures.end(); )
    {
        _body->DestroyFixture(*i);
        
        i = _fixtures.erase(i);
    }
    
    _groundSensorFixture = NULL;
}
