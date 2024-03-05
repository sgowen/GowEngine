//
//  World.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/RTTI.hpp"
#include "core/entity/EntityLayoutManager.hpp"

class World
{
    DECL_RTTI_NOPARENT;
    
public:
    World();
    virtual ~World();
    
    virtual void stepPhysics(float deltaTime) = 0;
    virtual void extrapolatePhysics(float extrapolation) = 0;
    virtual void endExtrapolatedPhysics() = 0;
    
    void populateFromEntityLayout(EntityLayout& eld);
    void addNetworkEntity(Entity* e);
    void removeNetworkEntity(Entity* e);
    void removeAllNetworkEntities();
    void storeToCache();
    void recallCache(uint32_t numMovesProcessed);
    void clearCache(uint32_t numMovesProcessed);
    void beginFrame();
    std::vector<Entity*> update();
    void reset();
    uint32_t rightEdge();
    uint32_t topEdge();
    
    bool isEntityLayoutLoaded();
    EntityLayout& getEntityLayout();
    std::vector<Entity*>& getLayers();
    std::vector<Entity*>& getStaticEntities();
    std::vector<Entity*>& getDynamicEntities();
    std::vector<Entity*>& getPlayers();
    Entity* getPlayer(uint8_t playerID);
    
    uint32_t getNumMovesProcessed();
    void resetNumMovesProcessed();
    
    Entity* getEntityByID(uint32_t id) const;
    
protected:
    EntityLayout _entityLayout;
    std::vector<Entity*> _layers;
    std::vector<Entity*> _staticEntities;
    std::vector<Entity*> _networkEntities;
    std::vector<Entity*> _players;
    
    virtual EntityControllerPhysics* createPhysicsController(Entity* e) = 0;
    
private:
    uint32_t _numMovesProcessed;
    uint32_t _rightEdge;
    uint32_t _topEdge;
    
    void addEntity(Entity* e);
    void removeEntity(Entity* e);
    void removeEntity(Entity* e, std::vector<Entity*>& entities);
    void removeAllEntities(std::vector<Entity*>& entities);
};
