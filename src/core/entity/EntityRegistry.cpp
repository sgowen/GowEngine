//
//  EntityRegistry.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 8/28/15.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
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
    auto q = _entityMap.find(id);
    
    if (q != _entityMap.end())
    {
        return q->second;
    }
    
    return nullptr;
}

void EntityRegistry::registerEntity(Entity* e)
{
    _entityMap.emplace(e->getID(), e);
    
    _onEntityRegisteredFunc(e);
}

void EntityRegistry::deregisterEntity(Entity* e)
{
    _entityMap.erase(e->getID());
    
    _onEntityDeregisteredFunc(e);
}

void EntityRegistry::deregisterAll()
{
    for (auto i = _entityMap.begin(); i != _entityMap.end(); )
    {
        Entity* e = i->second;
        
        i = _entityMap.erase(i);
        
        _onEntityDeregisteredFunc(e);
    }
}

std::map<uint32_t, Entity*>& EntityRegistry::getMap()
{
    return _entityMap;
}
