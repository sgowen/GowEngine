//
//  EntityRegistry.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 8/28/15.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

EntityRegistry::EntityRegistry(OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf) :
_onEntityRegisteredFunc(oerf),
_onEntityDeregisteredFunc(oedf)
{
    // Empty
}

Entity* EntityRegistry::getEntityByID(uint32_t id) const
{
    auto q = _entities.find(id);
    
    if (q != _entities.end())
    {
        return q->second;
    }
    
    return nullptr;
}

void EntityRegistry::registerEntity(Entity* e)
{
    _entities.emplace(e->getID(), e);
    
    _onEntityRegisteredFunc(e);
}

void EntityRegistry::deregisterEntity(Entity* e)
{
    _entities.erase(e->getID());
    
    _onEntityDeregisteredFunc(e);
}

void EntityRegistry::deregisterAll()
{
    for (auto i = _entities.begin(); i != _entities.end(); )
    {
        Entity* e = i->second;
        
        i = _entities.erase(i);
        
        _onEntityDeregisteredFunc(e);
    }
}

std::map<uint32_t, Entity*>& EntityRegistry::getMap()
{
    return _entities;
}
