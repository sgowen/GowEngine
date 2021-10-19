//
//  Box2DPhysicsWorld.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/entity/EntityLayoutManager.hpp"

struct TimeTracker;

class b2World;
class EntityContactListener;
class EntityContactFilter;

class Box2DPhysicsWorld
{
public:
    static bool s_isClient;
    static bool s_isLiveFrame;
    
    Box2DPhysicsWorld(TimeTracker& tt);
    ~Box2DPhysicsWorld();
    
    void populateFromEntityLayout(EntityLayoutDef& eld);
    void addNetworkEntity(Entity* e);
    void removeNetworkEntity(Entity* e);
    void recallCache();
    void stepPhysics();
    std::vector<Entity*> update();
    void reset();
    
    bool isEntityLayoutLoaded();
    EntityLayoutDef& getEntityLayout();
    std::vector<Entity*>& getLayers();
    std::vector<Entity*>& getStaticEntities();
    std::vector<Entity*>& getNetworkEntities();
    std::vector<Entity*>& getPlayers();
    b2World& getB2World();
    
private:
    TimeTracker& _timeTracker;
    b2World* _world;
    EntityContactListener* _entityContactListener;
    EntityContactFilter* _entityContactFilter;
    EntityLayoutDef _entityLayout;
    std::vector<Entity*> _layers;
    std::vector<Entity*> _staticEntities;
    std::vector<Entity*> _networkEntities;
    std::vector<Entity*> _players;
    
    void addEntity(Entity* e);
    void removeEntity(Entity* e);
    void removeEntity(Entity* e, std::vector<Entity*>& entities);
    void removeAllEntities(std::vector<Entity*>& entities);
};

#include <box2d/b2_world_callbacks.h>

class b2Contact;

class EntityContactListener : public b2ContactListener
{
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
};

class b2Fixture;

class EntityContactFilter : public b2ContactFilter
{
    virtual bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
};
