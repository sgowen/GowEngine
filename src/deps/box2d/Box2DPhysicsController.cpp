//
//  Box2DPhysicsController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <box2d/box2d.h>

IMPL_RTTI(Box2DPhysicsController, EntityPhysicsController)

Box2DPhysicsController::Box2DPhysicsController(Entity* e, b2World& world) : EntityPhysicsController(e),
_body(nullptr),
_groundSensorFixture(nullptr),
_damageSensorFixture(nullptr),
_numGroundContacts(0),
_isBodyFacingLeft(false)
{
    b2BodyDef bd;
    bd.position.Set(_entity->position()._x, _entity->position()._y);
    bd.type = _entity->isStatic() ? b2_staticBody : b2_dynamicBody;
    bd.fixedRotation = _entity->entityDef()._bodyFlags & BODF_FIXED_ROTATION;
    bd.userData.pointer = (uintptr_t)_entity;
    _body = world.CreateBody(&bd);
    
    createFixtures();
}

Box2DPhysicsController::~Box2DPhysicsController()
{
    destroyFixtures();
    
    b2World* world = _body->GetWorld();
    world->DestroyBody(_body);
    _body = nullptr;
}

Vector2 Box2DPhysicsController::velocity()
{
    b2Vec2 bodyVelocity = _body->GetLinearVelocity();
    return Vector2(bodyVelocity.x, bodyVelocity.y);
}

void Box2DPhysicsController::setVelocity(Vector2 v)
{
    _body->SetLinearVelocity(b2Vec2(v._x, v._y));
}

void Box2DPhysicsController::updatePoseFromBody()
{
    if (_body == nullptr)
    {
        return;
    }
    
    b2Vec2 bodyVelocity = _body->GetLinearVelocity();
    b2Vec2 bodyPosition = _body->GetPosition();
    
    _entity->velocity().set(bodyVelocity.x, bodyVelocity.y);
    _entity->position().set(bodyPosition.x, bodyPosition.y);
    _entity->pose()._numGroundContacts = _numGroundContacts;
}

void Box2DPhysicsController::updateBodyFromPose()
{
    if (_body == nullptr)
    {
        return;
    }
    
    b2Vec2 bodyVelocity = b2Vec2(_entity->velocity()._x, _entity->velocity()._y);
    b2Vec2 bodyPosition = b2Vec2(_entity->position()._x, _entity->position()._y);
    
    _body->SetLinearVelocity(bodyVelocity);
    _body->SetTransform(bodyPosition, 0);
    _numGroundContacts = _entity->pose()._numGroundContacts;
    
    if (_isBodyFacingLeft != _entity->isXFlipped())
    {
        destroyFixtures();
        createFixtures();
    }
}

bool Box2DPhysicsController::shouldCollide(Entity *e, b2Fixture* fixtureA, b2Fixture* fixtureB)
{
    if (fixtureA == _groundSensorFixture)
    {
        // Don't collide with myself
        return e != _entity;
    }
    
    return true;
}

void Box2DPhysicsController::handleBeginContact(Entity* e, b2Fixture* fixtureA, b2Fixture* fixtureB)
{
    if (fixtureA == _groundSensorFixture &&
        !fixtureB->IsSensor())
    {
        _numGroundContacts = CLAMP(_numGroundContacts + 1, 0, 15);
        
        if (ENGINE_CFG.physicsLoggingEnabled())
        {
            LOG("_numGroundContacts: %d", _numGroundContacts);
        }
    }
    
    if (fixtureA == _damageSensorFixture &&
        !fixtureB->IsSensor())
    {
        e->message(MSG_DAMAGE);
    }
}

void Box2DPhysicsController::handleEndContact(Entity* e, b2Fixture* fixtureA, b2Fixture* fixtureB)
{
    if (fixtureA == _groundSensorFixture &&
        !fixtureB->IsSensor())
    {
        _numGroundContacts = CLAMP(_numGroundContacts - 1, 0, 15);
        
        if (ENGINE_CFG.physicsLoggingEnabled())
        {
            LOG("_numGroundContacts: %d", _numGroundContacts);
        }
    }
}

b2Body* Box2DPhysicsController::getBody()
{
    return _body;
}

void Box2DPhysicsController::createFixtures()
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
        
        float wFactor = bodyWidth * fd._halfWidthFactor;
        float hFactor = bodyHeight * fd._halfHeightFactor;
        fd._center.set(fd._center._x * bodyWidth, fd._center._y * bodyHeight);
        
        b2Vec2 center = b2Vec2(fd._center._x, fd._center._y);
        
        b2PolygonShape shape;
        shape.SetAsBox(wFactor, hFactor, center, 0);
        
        b2FixtureDef b2fd;
        b2fd.shape = &shape;
        b2fd.isSensor = IS_BIT_SET(fd._flags, BFIXF_SENSOR);
        b2fd.density = 1.0f;
        b2fd.friction = 0.0f;
        b2fd.userData.pointer = (uintptr_t)_entity;
        
        b2Fixture* fixture = _body->CreateFixture(&b2fd);
        
        if (IS_BIT_SET(fd._flags, BFIXF_GROUND_SENSOR))
        {
            _groundSensorFixture = fixture;
        }
        
//        if (IS_BIT_SET(fd._flags, FIXF_DAMAGE_TOP))
//        {
//            _damageSensorFixture = fixture;
//        }
        
        _fixtures.push_back(fixture);
    }
}

void Box2DPhysicsController::destroyFixtures()
{
    for (std::vector<b2Fixture*>::iterator i = _fixtures.begin(); i != _fixtures.end(); )
    {
        _body->DestroyFixture(*i);
        
        i = _fixtures.erase(i);
    }
    
    _groundSensorFixture = nullptr;
    _damageSensorFixture = nullptr;
}
