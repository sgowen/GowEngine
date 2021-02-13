//
//  EntityManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/5/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Entity.hpp"

#include <map>
#include <vector>
#include <string>

struct EntityInstanceDef;
class EntityController;
class EntityPhysicsController;
class EntityNetworkController;

typedef EntityController* (*EntityControllerCreationFunc)(Entity* e);
typedef EntityPhysicsController* (*EntityPhysicsControllerCreationFunc)(Entity* e);
typedef EntityNetworkController* (*EntityNetworkControllerCreationFunc)(Entity* e, bool isServer);

#define ENTITY_MGR EntityManager::getInstance()

class EntityManager
{
public:
    static EntityManager& getInstance()
    {
        static EntityManager ret = EntityManager();
        return ret;
    }
    
    void initWithJSONFile(const char* filePath);
    void initWithJSON(const char* json);
    
    void clear();
    
    Entity* createEntity(EntityInstanceDef& eid, bool isServer);
    EntityDef& getEntityDef(uint32_t fourCCName);
    
    void registerController(std::string name, EntityControllerCreationFunc func);
    void registerNetworkController(std::string name, EntityNetworkControllerCreationFunc func);
    void registerPhysicsController(std::string name, EntityPhysicsControllerCreationFunc func);
    
    EntityController* createEntityController(EntityDef& ed, Entity* e);
    EntityNetworkController* createEntityNetworkController(EntityDef& ed, Entity* e, bool isServer);
    EntityPhysicsController* createEntityPhysicsController(EntityDef& ed, Entity* e);
    
    const std::map<std::string, EntityControllerCreationFunc>& getEntityControllerMap();
    const std::map<std::string, EntityNetworkControllerCreationFunc>& getEntityNetworkControllerMap();
    const std::map<std::string, EntityPhysicsControllerCreationFunc>& getEntityPhysicsControllerMap();
    
private:
    std::map<uint32_t, EntityDef> _entityDescriptorsMap;
    std::map<std::string, EntityControllerCreationFunc> _entityControllerCreationFunctionMap;
    std::map<std::string, EntityNetworkControllerCreationFunc> _entityNetworkControllerCreationFunctionMap;
    std::map<std::string, EntityPhysicsControllerCreationFunc> _entityPhysicsControllerCreationFunctionMap;
    
    EntityManager();
    ~EntityManager() {}
    EntityManager(const EntityManager&);
    EntityManager& operator=(const EntityManager&);
};
