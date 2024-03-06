//
//  Box2DControllerPhysics.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <box2d/box2d.h>

IMPL_RTTI(Box2DControllerPhysics, EntityControllerPhysics)

Box2DControllerPhysics::Box2DControllerPhysics(Entity* e, b2World& world) : EntityControllerPhysics(e),
_body(nullptr),
_groundSensorFixture(nullptr),
_damageSensorFixture(nullptr),
_numGroundContacts(0),
_isBodyFacingLeft(false)
{
    b2BodyDef bd;
    bd.position.Set(_entity->position()._x, _entity->position()._y);
    bd.type = _entity->isDynamic() ? b2_dynamicBody : b2_staticBody;
    bd.fixedRotation = _entity->entityDef()._bodyFlags & BODF_FIXED_ROTATION;
    bd.userData.pointer = (uintptr_t)_entity;
    _body = world.CreateBody(&bd);
    
    createFixtures();
}

Box2DControllerPhysics::~Box2DControllerPhysics()
{
    destroyFixtures();
    
    b2World* world = _body->GetWorld();
    world->DestroyBody(_body);
    _body = nullptr;
}

void Box2DControllerPhysics::updateBodyFromPose()
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

void Box2DControllerPhysics::updatePoseFromBody()
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

bool Box2DControllerPhysics::shouldCollide(Entity *e, b2Fixture* fixtureA, b2Fixture* fixtureB)
{
    if (fixtureA == _groundSensorFixture)
    {
        // Don't collide with myself
        return e != _entity;
    }
    
    return true;
}

void Box2DControllerPhysics::handleBeginContact(Entity* e, b2Fixture* fixtureA, b2Fixture* fixtureB)
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
        Entity* entityA = (Entity*)fixtureA->GetUserData().pointer;
        entityA->message(MSG_DANGEROUS_TOUCH, e);
    }
}

void Box2DControllerPhysics::handleEndContact(Entity* e, b2Fixture* fixtureA, b2Fixture* fixtureB)
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
    
    if (fixtureA == _damageSensorFixture &&
        !fixtureB->IsSensor())
    {
        Entity* entityA = (Entity*)fixtureA->GetUserData().pointer;
        entityA->message(MSG_NO_TOUCH, e);
    }
}

b2Body* Box2DControllerPhysics::getBody()
{
    return _body;
}

void Box2DControllerPhysics::createFixtures()
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
        
        b2Shape* shape;
        b2PolygonShape polygonShape;
        b2CircleShape circleShape;
        if (fd._flags & BFIXF_CIRCLE)
        {
            circleShape.m_p.Set(fd._center._x * bodyWidth, fd._center._y * bodyHeight);
            circleShape.m_radius = fd._vertices[0]._x * bodyWidth;
            
            shape = &circleShape;
        }
        else
        {
            if (fd._flags & BFIXF_BOX)
            {
                float wFactor = bodyWidth * fd._vertices[0]._x;
                float hFactor = bodyHeight * fd._vertices[0]._y;
                
                b2Vec2 center;
                center.Set(fd._center._x * bodyWidth, fd._center._y * bodyHeight);
                polygonShape.SetAsBox(wFactor, hFactor, center, 0);
            }
            else
            {
                std::vector<b2Vec2> b2Vertices;
                for (std::vector<Vector2>::iterator i = fd._vertices.begin(); i != fd._vertices.end(); ++i)
                {
                    Vector2& vertex = (*i);
                    b2Vertices.emplace_back(vertex._x * bodyWidth, vertex._y * bodyHeight);
                }
                
                int count = static_cast<int>(b2Vertices.size());
                polygonShape.Set(&b2Vertices[0], count);
            }
            shape = &polygonShape;
        }
        
        b2FixtureDef b2fd;
        b2fd.shape = shape;
        b2fd.isSensor = IS_BIT_SET(fd._flags, BFIXF_SENSOR);
        b2fd.density = fd._density;
        b2fd.friction = fd._friction;
        b2fd.restitution = fd._restitution;
        b2fd.userData.pointer = (uintptr_t)_entity;
        
        b2Fixture* fixture = _body->CreateFixture(&b2fd);
        
        if (IS_BIT_SET(fd._flags, BFIXF_GROUND_SENSOR))
        {
            _groundSensorFixture = fixture;
        }
        
        if (IS_BIT_SET(fd._flags, BFIXF_DAMAGE_SENSOR))
        {
            _damageSensorFixture = fixture;
        }
        
        _fixtures.push_back(fixture);
    }
}

void Box2DControllerPhysics::destroyFixtures()
{
    for (std::vector<b2Fixture*>::iterator i = _fixtures.begin(); i != _fixtures.end(); )
    {
        _body->DestroyFixture(*i);
        
        i = _fixtures.erase(i);
    }
    
    _groundSensorFixture = nullptr;
    _damageSensorFixture = nullptr;
}
