//
//  World.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/RTTI.hpp"
#include "core/entity/EntityLayoutManager.hpp"

class World
{
    DECL_RTTI_NOPARENT;
    
public:
    static World* create();
    
    World();
    virtual ~World();
    
    virtual void stepPhysics(float deltaTime) = 0;
    virtual void interpolatePhysics(float interpolation) = 0;
    virtual void endInterpolatedPhysics() = 0;
    
    void populateFromEntityLayout(EntityLayoutDef& eld);
    void addNetworkEntity(Entity* e);
    void removeNetworkEntity(Entity* e);
    void removeAllNetworkEntities();
    void storeToCache();
    void recallCache(uint32_t numMovesProcessed);
    void beginFrame();
    std::vector<Entity*> update();
    void reset();
    
    bool isEntityLayoutLoaded();
    EntityLayoutDef& getEntityLayout();
    std::vector<Entity*>& getLayers();
    std::vector<Entity*>& getStaticEntities();
    std::vector<Entity*>& getNetworkEntities();
    std::vector<Entity*>& getPlayers();
    
    uint32_t getNumMovesProcessed();
    void resetNumMovesProcessed();
    
protected:
    EntityLayoutDef _entityLayout;
    std::vector<Entity*> _layers;
    std::vector<Entity*> _staticEntities;
    std::vector<Entity*> _networkEntities;
    std::vector<Entity*> _players;
    
    virtual EntityPhysicsController* createPhysicsController(Entity* e) = 0;
    
private:
    uint32_t _numMovesProcessed;
    
    void addEntity(Entity* e);
    void removeEntity(Entity* e);
    void removeEntity(Entity* e, std::vector<Entity*>& entities);
    void removeAllEntities(std::vector<Entity*>& entities);
};
