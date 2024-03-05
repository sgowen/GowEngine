//
//  Box2DPhysicsWorld.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <box2d/box2d.h>

IMPL_RTTI(Box2DPhysicsWorld, World)

Box2DPhysicsWorld::Box2DPhysicsWorld() : World(),
_world(new b2World(b2Vec2(0.0f, -120))),
_entityContactListener(new EntityContactListener()),
_entityContactFilter(new EntityContactFilter())
{
    _world->SetContactListener(_entityContactListener);
    _world->SetContactFilter(_entityContactFilter);
}

Box2DPhysicsWorld::~Box2DPhysicsWorld()
{
    // TODO, fix this, I don't like this one bit.
    // reset is called in the super destructor, World
    // Buuuut, because we delete _world here, the entities that are released
    // in the World destructor AFTER this crash because Box2D has already been shut down
    reset();
    
    delete _entityContactListener;
    delete _entityContactFilter;
    delete _world;
}

void Box2DPhysicsWorld::stepPhysics(float deltaTime)
{
    static int32 velocityIterations = 4;
    static int32 positionIterations = 2;
    
    // Instruct the world to perform a single step of simulation.
    // It is generally best to keep the time step and iterations fixed.
    _world->Step(deltaTime, velocityIterations, positionIterations);
    
    for (Entity* e : _players)
    {
        Box2DPhysicsController* epc = e->physicsController<Box2DPhysicsController>();
        epc->updatePoseFromBody();
    }
    
    for (Entity* e : _networkEntities)
    {
        Box2DPhysicsController* epc = e->physicsController<Box2DPhysicsController>();
        epc->updatePoseFromBody();
    }
}

void Box2DPhysicsWorld::extrapolatePhysics(float extrapolation)
{
    // TODO
}

void Box2DPhysicsWorld::endExtrapolatedPhysics()
{
    // TODO
}

EntityControllerPhysics* Box2DPhysicsWorld::createPhysicsController(Entity* e)
{
    return new Box2DPhysicsController(e, *_world);
}

b2World& Box2DPhysicsWorld::getB2World()
{
    return *_world;
}

void EntityContactListener::BeginContact(b2Contact* contact)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    
    Entity* entityA = (Entity*)fixtureA->GetUserData().pointer;
    Entity* entityB = (Entity*)fixtureB->GetUserData().pointer;
    
    Box2DPhysicsController* entityAPC = entityA->physicsController<Box2DPhysicsController>();
    Box2DPhysicsController* entityBPC = entityB->physicsController<Box2DPhysicsController>();
    
    entityAPC->handleBeginContact(entityB, fixtureA, fixtureB);
    entityBPC->handleBeginContact(entityA, fixtureB, fixtureA);
}

void EntityContactListener::EndContact(b2Contact* contact)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    
    Entity* entityA = (Entity*)fixtureA->GetUserData().pointer;
    Entity* entityB = (Entity*)fixtureB->GetUserData().pointer;
    
    Box2DPhysicsController* entityAPC = entityA->physicsController<Box2DPhysicsController>();
    Box2DPhysicsController* entityBPC = entityB->physicsController<Box2DPhysicsController>();
    
    entityAPC->handleEndContact(entityB, fixtureA, fixtureB);
    entityBPC->handleEndContact(entityA, fixtureB, fixtureA);
}

bool EntityContactFilter::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
{
    Entity* entityA = (Entity*)fixtureA->GetUserData().pointer;
    Entity* entityB = (Entity*)fixtureB->GetUserData().pointer;
    
    Box2DPhysicsController* entityAPC = entityA->physicsController<Box2DPhysicsController>();
    Box2DPhysicsController* entityBPC = entityB->physicsController<Box2DPhysicsController>();
    
    bool entityAPCShouldCollide = entityAPC->shouldCollide(entityB, fixtureA, fixtureB);
    bool entityBPCShouldCollide = entityBPC->shouldCollide(entityA, fixtureB, fixtureA);
    
    return entityAPCShouldCollide && entityBPCShouldCollide;
}
