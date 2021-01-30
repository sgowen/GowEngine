//
//  EntityManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 8/28/15.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <map>

class Entity;

typedef void (*HandleEntityCreatedFunc)(Entity* e);
typedef void (*HandleEntityDeletionFunc)(Entity* e);

class EntityManager
{
public:
    EntityManager(HandleEntityCreatedFunc handleEntityCreatedFunc, HandleEntityDeletionFunc handleEntityDeletionFunc);
    ~EntityManager() {}
    
    Entity* getEntityByID(uint32_t id) const;
    void registerEntity(Entity* e);
    void deregisterEntity(Entity* e);
    void setEntityID(uint32_t value);
    std::map<uint32_t, Entity*>& getMap();
    
private:
    HandleEntityCreatedFunc _handleEntityCreatedFunc;
    HandleEntityDeletionFunc _handleEntityDeletionFunc;
    
    std::map<uint32_t, Entity*> _entityMap;
};
