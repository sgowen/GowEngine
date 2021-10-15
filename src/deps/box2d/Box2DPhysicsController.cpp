//
//  Box2DPhysicsController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <box2d/box2d.h>

IMPL_RTTI(Box2DPhysicsController, EntityPhysicsController)
IMPL_EntityPhysicsController_create(Box2DPhysicsController)

Box2DPhysicsController::Box2DPhysicsController(Entity* e) : EntityPhysicsController(e),
_body(nullptr),
_groundSensorFixture(nullptr),
_bodyWidth(0),
_bodyHeight(0),
_isBodyFacingLeft(false)
{
    // Empty
}

Box2DPhysicsController::~Box2DPhysicsController()
{
    // Empty
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
    _entity->pose()._angle = _body->GetAngle();
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
    _body->SetTransform(bodyPosition, _entity->angle());
    
    if (_isBodyFacingLeft != _entity->isXFlipped() ||
        _bodyWidth != _entity->width() ||
        _bodyHeight != _entity->height())
    {
        destroyFixtures();
        createFixtures();
    }
}

bool Box2DPhysicsController::shouldCollide(Entity *e, b2Fixture* fixtureA, b2Fixture* fixtureB)
{
    if (fixtureA == _groundSensorFixture)
    {
        // Don't collide with yourself
        return e != _entity;
    }
    
    return true;
}

void Box2DPhysicsController::handleBeginContact(Entity* e, b2Fixture* fixtureA, b2Fixture* fixtureB)
{
    if (fixtureA == _groundSensorFixture &&
        !fixtureB->IsSensor())
    {
        _entity->pose()._numGroundContacts = CLAMP(_entity->pose()._numGroundContacts + 1, 0, 15);
    }
}

void Box2DPhysicsController::handleEndContact(Entity* e, b2Fixture* fixtureA, b2Fixture* fixtureB)
{
    if (fixtureA == _groundSensorFixture &&
        !fixtureB->IsSensor())
    {
        _entity->pose()._numGroundContacts = CLAMP(_entity->pose()._numGroundContacts - 1, 0, 15);
    }
}

void Box2DPhysicsController::initPhysics(b2World& world)
{
    assert(_body == nullptr);
    
    b2BodyDef bodyDef;
    bodyDef.position.Set(_entity->position()._x, _entity->position()._y);
    bodyDef.type = _entity->isStatic() ? b2_staticBody : b2_dynamicBody;
    bodyDef.fixedRotation = _entity->isFixedRotation();
    bodyDef.userData.pointer = (uintptr_t)_entity;
    _body = world.CreateBody(&bodyDef);
    
    createFixtures();
}

void Box2DPhysicsController::deinitPhysics()
{
    assert(_body != nullptr);
    
    destroyFixtures();
    
    b2World* world = _body->GetWorld();
    world->DestroyBody(_body);
    _body = nullptr;
}

b2Body* Box2DPhysicsController::getBody()
{
    return _body;
}

void Box2DPhysicsController::createFixtures()
{
    _isBodyFacingLeft = _entity->isXFlipped();
    _bodyWidth = _entity->width();
    _bodyHeight = _entity->height();
    
    for (std::vector<FixtureDef>::iterator i = _entity->entityDef()._fixtures.begin(); i != _entity->entityDef()._fixtures.end(); ++i)
    {
        FixtureDef fd = *i;
        if (_isBodyFacingLeft)
        {
            fd._center._x = -fd._center._x;
        }
        
        b2Shape* shape;
        b2PolygonShape polygonShape;
        b2CircleShape circleShape;
        if (IS_BIT_SET(fd._flags, FIXF_CIRCLE))
        {
            circleShape.m_p.Set(fd._center._x * _bodyWidth, fd._center._y * _bodyHeight);
            circleShape.m_radius = fd._vertices[0]._x * _bodyWidth;
            
            shape = &circleShape;
        }
        else
        {
            if (IS_BIT_SET(fd._flags, FIXF_BOX))
            {
                float wFactor = _bodyWidth * fd._vertices[0]._x;
                float hFactor = _bodyHeight * fd._vertices[0]._y;
                fd._center.set(fd._center._x * _bodyWidth, fd._center._y * _bodyHeight);
                
                b2Vec2 center = b2Vec2(fd._center._x, fd._center._y);
                polygonShape.SetAsBox(wFactor, hFactor, center, 0);
            }
            else
            {
                std::vector<b2Vec2> bodyVertices;
                for (std::vector<Vector2>::iterator i = fd._vertices.begin(); i != fd._vertices.end(); ++i)
                {
                    Vector2& vertex = (*i);
                    vertex.set(vertex._x * _bodyWidth, vertex._y * _bodyHeight);
                    bodyVertices.emplace_back(vertex._x, vertex._y);
                }
                
                int count = static_cast<int>(bodyVertices.size());
                polygonShape.Set(&bodyVertices[0], count);
            }
            shape = &polygonShape;
        }
        
        b2FixtureDef b2fd;
        b2fd.shape = shape;
        b2fd.isSensor = IS_BIT_SET(fd._flags, FIXF_SENSOR);
        b2fd.density = fd._density;
        b2fd.friction = fd._friction;
        b2fd.restitution = fd._restitution;
        b2fd.userData.pointer = (uintptr_t)_entity;
        
        b2Fixture* fixture = _body->CreateFixture(&b2fd);
        
        if (IS_BIT_SET(fd._flags, FIXF_GROUND_CONTACT))
        {
            _groundSensorFixture = fixture;
        }
        
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
}
