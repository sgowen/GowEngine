//
//  EntityManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 8/28/15.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "EntityManager.hpp"

#include "Entity.hpp"

EntityManager::EntityManager(HandleEntityCreatedFunc handleEntityCreatedFunc, HandleEntityDeletionFunc handleEntityDeletionFunc) :
_handleEntityCreatedFunc(handleEntityCreatedFunc),
_handleEntityDeletionFunc(handleEntityDeletionFunc)
{
    // Empty
}

Entity* EntityManager::getEntityByID(uint32_t id) const
{
    auto q = _entityMap.find(id);
    
    if (q != _entityMap.end())
    {
        return q->second;
    }
    
    return NULL;
}

void EntityManager::registerEntity(Entity* e)
{
    _entityMap.insert(std::make_pair(e->getID(), e));
    
    if (_handleEntityCreatedFunc)
    {
        _handleEntityCreatedFunc(e);
    }
}

void EntityManager::deregisterEntity(Entity* e)
{
    _entityMap.erase(e->getID());
    
    if (_handleEntityDeletionFunc)
    {
        _handleEntityDeletionFunc(e);
    }
}

std::map<uint32_t, Entity*>& EntityManager::getMap()
{
    return _entityMap;
}
