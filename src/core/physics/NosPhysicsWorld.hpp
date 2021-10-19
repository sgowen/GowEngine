//
//  NosPhysicsWorld.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/entity/EntityLayoutManager.hpp"

struct TimeTracker;

class EntityContactListener;
class EntityContactFilter;

class NosPhysicsWorld
{
public:
    static bool s_isClient;
    static bool s_isLiveFrame;
    
    NosPhysicsWorld(TimeTracker& tt);
    ~NosPhysicsWorld();
    
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
    
private:
    TimeTracker& _timeTracker;
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
