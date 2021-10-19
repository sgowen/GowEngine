//
//  Box2DPhysicsWorld.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <box2d/box2d.h>

bool Box2DPhysicsWorld::s_isClient = false;
bool Box2DPhysicsWorld::s_isLiveFrame = false;

Box2DPhysicsWorld::Box2DPhysicsWorld(TimeTracker& tt) :
_timeTracker(tt),
_world(new b2World(b2Vec2(0.0f, -120))),
_entityContactListener(new EntityContactListener()),
_entityContactFilter(new EntityContactFilter()),
_entityLayout()
{
    _world->SetContactListener(_entityContactListener);
    _world->SetContactFilter(_entityContactFilter);
}

Box2DPhysicsWorld::~Box2DPhysicsWorld()
{
    reset();
    
    delete _entityContactListener;
    delete _entityContactFilter;
    delete _world;
}

void Box2DPhysicsWorld::populateFromEntityLayout(EntityLayoutDef& eld)
{
    reset();
    
    _entityLayout = eld;
    
    for (auto& eid : _entityLayout._entities)
    {
        addEntity(ENTITY_MGR.createEntity(eid));
    }
}

void Box2DPhysicsWorld::addNetworkEntity(Entity* e)
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

void Box2DPhysicsWorld::removeNetworkEntity(Entity* e)
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

void Box2DPhysicsWorld::recallCache()
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

void Box2DPhysicsWorld::stepPhysics()
{
    static int32 velocityIterations = 4;
    static int32 positionIterations = 2;
    
    // Instruct the world to perform a single step of simulation.
    // It is generally best to keep the time step and iterations fixed.
    _world->Step(FRAME_RATE_PHYSICS, velocityIterations, positionIterations);
    if (TIME_SCALE == 1)
    {
        // FRAME_RATE_PHYSICS is 60 FPS
        // so need to step again when running
        // in 30 FPS mode
        _world->Step(FRAME_RATE_PHYSICS, velocityIterations, positionIterations);
    }
}

std::vector<Entity*> Box2DPhysicsWorld::update()
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

void Box2DPhysicsWorld::reset()
{
    _entityLayout = EntityLayoutDef();
    
    removeAllEntities(_layers);
    removeAllEntities(_staticEntities);
}

bool Box2DPhysicsWorld::isEntityLayoutLoaded()
{
    return _entityLayout._key > 0;
}

EntityLayoutDef& Box2DPhysicsWorld::getEntityLayout()
{
    return _entityLayout;
}

std::vector<Entity*>& Box2DPhysicsWorld::getLayers()
{
    return _layers;
}

std::vector<Entity*>& Box2DPhysicsWorld::getStaticEntities()
{
    return _staticEntities;
}

std::vector<Entity*>& Box2DPhysicsWorld::getNetworkEntities()
{
    return _networkEntities;
}

std::vector<Entity*>& Box2DPhysicsWorld::getPlayers()
{
    return _players;
}

b2World& Box2DPhysicsWorld::getB2World()
{
    return *_world;
}

void Box2DPhysicsWorld::addEntity(Entity *e)
{
    assert(!e->isPlayer() && !e->isDynamic());
    
    if (e->isLayer())
    {
        _layers.push_back(e);
    }
    else if (e->isStatic())
    {
        Box2DPhysicsController* epc = e->physicsController<Box2DPhysicsController>();
        epc->initPhysics(*_world);
        
        _staticEntities.push_back(e);
    }
}

void Box2DPhysicsWorld::removeEntity(Entity* e)
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

void Box2DPhysicsWorld::removeEntity(Entity* e, std::vector<Entity*>& entities)
{
    assert(e != nullptr);
    
    for (std::vector<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
    {
        Entity* entityToDelete = *i;
        if (entityToDelete == e)
        {
            if (!entityToDelete->isLayer())
            {
                Box2DPhysicsController* epc = entityToDelete->physicsController<Box2DPhysicsController>();
                epc->deinitPhysics();
            }
            
            delete entityToDelete;
            
            entities.erase(i);
            return;
        }
    }
}

void Box2DPhysicsWorld::removeAllEntities(std::vector<Entity*>& entities)
{
    for (std::vector<Entity*>::iterator i = entities.begin(); i != entities.end(); )
    {
        Entity* entityToDelete = *i;
        if (!entityToDelete->isLayer())
        {
            Box2DPhysicsController* epc = entityToDelete->physicsController<Box2DPhysicsController>();
            epc->deinitPhysics();
        }
        
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
