//
//  Box2DWorld.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <box2d/box2d.h>

Box2DWorld::Box2DWorld() :
_world(new b2World(b2Vec2(0.0f, -44))),
_entityContactListener(new EntityContactListener()),
_entityContactFilter(new EntityContactFilter()),
_entityLayout()
{
    _world->SetContactListener(_entityContactListener);
    _world->SetContactFilter(_entityContactFilter);
}

Box2DWorld::~Box2DWorld()
{
    reset();
    
    delete _entityContactListener;
    delete _entityContactFilter;
    delete _world;
}

void Box2DWorld::populateFromEntityLayout(EntityLayoutDef& eld)
{
    reset();
    
    _entityLayout = eld;
    
    for (auto& eid : _entityLayout._entities)
    {
        addEntity(ENTITY_MGR.createEntity(eid));
    }
}

void Box2DWorld::addNetworkEntity(Entity* e)
{
    assert(!e->isLayer() && !e->isStatic());
    
    Box2DPhysicsController* epc = e->physicsController<Box2DPhysicsController>();
    epc->initPhysics(*_world);
    
    if (e->isPlayer())
    {
        _players.push_back(e);
    }
    else if (e->isDynamic())
    {
        _networkEntities.push_back(e);
    }
}

void Box2DWorld::removeNetworkEntity(Entity* e)
{
    assert(!e->isLayer() && !e->isStatic());
    
    if (e->isPlayer())
    {
        removeEntity(e, _players);
    }
    else if (e->isDynamic())
    {
        removeEntity(e, _networkEntities);
    }
}

void Box2DWorld::recallCache()
{
    for (Entity* e : _players)
    {
        e->networkController()->recallCache();
    }
    
    for (Entity* e : _networkEntities)
    {
        e->networkController()->recallCache();
    }
}

void Box2DWorld::stepPhysics(TimeTracker* tt)
{
    static int32 velocityIterations = 6;
    static int32 positionIterations = 2;
    
    // Instruct the world to perform a single step of simulation.
    // It is generally best to keep the time step and iterations fixed.
    _world->Step(tt->_frameRate, velocityIterations, positionIterations);
}

std::vector<Entity*> Box2DWorld::update()
{
    std::vector<Entity*> toDelete;
    for (Entity* e : _players)
    {
        e->update();
        if (e->isRequestingDeletion())
        {
            toDelete.push_back(e);
        }
    }
    
    for (Entity* e : _networkEntities)
    {
        e->update();
        if (e->isRequestingDeletion())
        {
            toDelete.push_back(e);
        }
    }
    
    return toDelete;
}

void Box2DWorld::reset()
{
    _entityLayout = EntityLayoutDef();
    
    removeAllEntities(_layers);
    removeAllEntities(_staticEntities);
}

bool Box2DWorld::isEntityLayoutLoaded()
{
    return _entityLayout._key > 0;
}

EntityLayoutDef& Box2DWorld::getEntityLayout()
{
    return _entityLayout;
}

std::vector<Entity*>& Box2DWorld::getLayers()
{
    return _layers;
}

std::vector<Entity*>& Box2DWorld::getStaticEntities()
{
    return _staticEntities;
}

std::vector<Entity*>& Box2DWorld::getNetworkEntities()
{
    return _networkEntities;
}

std::vector<Entity*>& Box2DWorld::getPlayers()
{
    return _players;
}

b2World& Box2DWorld::getB2World()
{
    return *_world;
}

void Box2DWorld::addEntity(Entity *e)
{
    assert(!e->isPlayer() && !e->isDynamic());
    
    Box2DPhysicsController* epc = e->physicsController<Box2DPhysicsController>();
    epc->initPhysics(*_world);
    
    if (e->isLayer())
    {
        _layers.push_back(e);
    }
    else if (e->isStatic())
    {
        _staticEntities.push_back(e);
    }
}

void Box2DWorld::removeEntity(Entity* e)
{
    assert(!e->isDynamic());
    
    if (e->isLayer())
    {
        removeEntity(e, _layers);
    }
    else if (e->isStatic())
    {
        removeEntity(e, _staticEntities);
    }
}

void Box2DWorld::removeEntity(Entity* e, std::vector<Entity*>& entities)
{
    assert(e != nullptr);
    
    for (std::vector<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
    {
        Entity* entityToDelete = *i;
        if (entityToDelete == e)
        {
            Box2DPhysicsController* epc = entityToDelete->physicsController<Box2DPhysicsController>();
            epc->deinitPhysics();
            
            delete entityToDelete;
            
            entities.erase(i);
            return;
        }
    }
}

void Box2DWorld::removeAllEntities(std::vector<Entity*>& entities)
{
    for (std::vector<Entity*>::iterator i = entities.begin(); i != entities.end(); )
    {
        Entity* entityToDelete = *i;
        Box2DPhysicsController* epc = entityToDelete->physicsController<Box2DPhysicsController>();
        epc->deinitPhysics();
        
        delete entityToDelete;
        
        i = entities.erase(i);
    }
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
