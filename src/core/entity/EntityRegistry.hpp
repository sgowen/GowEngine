//
//  EntityRegistry.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 8/28/15.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <map>

class Entity;

typedef void (*OnEntityRegisteredFunc)(Entity* e);
typedef void (*OnEntityDeregisteredFunc)(Entity* e);

class EntityRegistry
{
public:
    EntityRegistry(OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf);
    
    Entity* getEntityByID(uint32_t id) const;
    void registerEntity(Entity* e);
    void deregisterEntity(Entity* e);
    void deregisterAll();
    std::map<uint32_t, Entity*>& getMap();
    
private:
    OnEntityRegisteredFunc _onEntityRegisteredFunc;
    OnEntityDeregisteredFunc _onEntityDeregisteredFunc;
    
    std::map<uint32_t, Entity*> _entities;
};
