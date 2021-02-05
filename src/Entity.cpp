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
#include "Macros.hpp"
#include "Rektangle.hpp"
#include "OverlapTester.hpp"

Entity::Entity(EntityDef ed) :
_entityDef(ed),
_controller(ENTITY_MAPPER.createEntityController(ed, this)),
_networkController(ENTITY_MAPPER.createEntityNetworkController(ed, this)),
_body(NULL),
_groundSensorFixture(NULL),
_pose(ed._x, ed._y),
_poseNetworkCache(_pose),
_ID(ed._entityID),
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
    
    if (IS_BIT_SET(_entityDef._bodyFlags, BODF_DYNAMIC))
    {
        ++_state._stateTime;
    }
    
    _controller->update();
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
    bodyDef.position.Set(_pose._position._x, _pose._position._y);
    bodyDef.type = IS_BIT_SET(_entityDef._bodyFlags, BODF_STATIC) ? b2_staticBody : b2_dynamicBody;
    bodyDef.fixedRotation = IS_BIT_SET(_entityDef._bodyFlags, BODF_FIXED_ROTATION);
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
    
    b2Vec2 bodyVelocity = _body->GetLinearVelocity();
    b2Vec2 bodyPosition = _body->GetPosition();
    _pose._velocity.set(bodyVelocity.x, bodyVelocity.y);
    _pose._position.set(bodyPosition.x, bodyPosition.y);
    _pose._angle = _body->GetAngle();
}

void Entity::updateBodyFromPose()
{
    if (_body == NULL)
    {
        return;
    }
    
    b2Vec2 bodyVelocity = b2Vec2(_pose._velocity._x, _pose._velocity._y);
    b2Vec2 bodyPosition = b2Vec2(_pose._position._x, _pose._position._y);
    
    _body->SetLinearVelocity(bodyVelocity);
    _body->SetTransform(bodyPosition, _pose._angle);
    
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

void Entity::setPosition(Vector2 position)
{
    setPosition(position._x, position._y);
}

void Entity::setPosition(float x, float y)
{
    _pose._position.set(x, y);
    
    updateBodyFromPose();
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
    
    updateBodyFromPose();
}

const Vector2& Entity::getVelocity()
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
    
    return _controller->getTextureMapping(state);
}

int Entity::getSoundMapping(int state)
{
    {
        auto q = _entityDef._soundMappings.find(state);
        if (q != _entityDef._soundMappings.end())
        {
            return q->second;
        }
    }
    
    {
        auto q = _entityDef._soundRandomMappings.find(state);
        if (q != _entityDef._soundRandomMappings.end())
        {
            std::vector<int> soundCollection = q->second;
            
            int index = rand() % soundCollection.size();
            return soundCollection[index];
        }
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
            def._center._x = -def._center._x;
        }
        
        b2Shape* shape;
        b2PolygonShape polygonShape;
        b2CircleShape circleShape;
        if (IS_BIT_SET(def._flags, FIXF_CIRCLE))
        {
            circleShape.m_p.Set(def._center._x * _entityDef._width, def._center._y * _entityDef._height);
            circleShape.m_radius = def._vertices[0]._x * _entityDef._width;
            
            shape = &circleShape;
        }
        else
        {
            if (IS_BIT_SET(def._flags, FIXF_BOX))
            {
                float wFactor = _entityDef._width * def._vertices[0]._x;
                float hFactor = _entityDef._height * def._vertices[0]._y;
                def._center.set(def._center._x * _entityDef._width, def._center._y * _entityDef._height);
                
                b2Vec2 center = b2Vec2(def._center._x, def._center._y);
                polygonShape.SetAsBox(wFactor, hFactor, center, 0);
            }
            else
            {
                std::vector<b2Vec2> bodyVertices;
                for (std::vector<Vector2>::iterator i = def._vertices.begin(); i != def._vertices.end(); ++i)
                {
                    Vector2& vertex = (*i);
                    vertex.set(vertex._x * _entityDef._width, vertex._y * _entityDef._height);
                    bodyVertices.emplace_back(vertex._x, vertex._y);
                }
                
                int count = static_cast<int>(bodyVertices.size());
                polygonShape.Set(&bodyVertices[0], count);
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
