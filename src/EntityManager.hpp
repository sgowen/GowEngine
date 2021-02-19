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
class EntityNetworkController;
class EntityPhysicsController;
class EntityRenderController;

typedef EntityController* (*EntityControllerCreationFunc)(Entity* e);
typedef EntityNetworkController* (*EntityNetworkControllerCreationFunc)(Entity* e);
typedef EntityPhysicsController* (*EntityPhysicsControllerCreationFunc)(Entity* e);
typedef EntityRenderController* (*EntityRenderControllerCreationFunc)(Entity* e);

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
    Entity* createEntity(EntityInstanceDef eid);
    EntityDef& getEntityDef(uint32_t fourCCName);
    
    void registerController(std::string name, EntityControllerCreationFunc func);
    EntityController* createEntityController(EntityDef& ed, Entity* e);
    const std::map<std::string, EntityControllerCreationFunc>& getEntityControllerMap();
    
    void registerNetworkController(std::string name, EntityNetworkControllerCreationFunc func);
    EntityNetworkController* createEntityNetworkController(EntityDef& ed, Entity* e);
    const std::map<std::string, EntityNetworkControllerCreationFunc>& getEntityNetworkControllerMap();
    
    void registerPhysicsController(std::string name, EntityPhysicsControllerCreationFunc func);
    EntityPhysicsController* createEntityPhysicsController(EntityDef& ed, Entity* e);
    const std::map<std::string, EntityPhysicsControllerCreationFunc>& getEntityPhysicsControllerMap();
    
    void registerRenderController(std::string name, EntityRenderControllerCreationFunc func);
    EntityRenderController* createEntityRenderController(EntityDef& ed, Entity* e);
    const std::map<std::string, EntityRenderControllerCreationFunc>& getEntityRenderControllerMap();
    
private:
    std::map<uint32_t, EntityDef> _entityDescriptorsMap;
    std::map<std::string, EntityControllerCreationFunc> _entityControllerCreationFunctionMap;
    std::map<std::string, EntityNetworkControllerCreationFunc> _entityNetworkControllerCreationFunctionMap;
    std::map<std::string, EntityPhysicsControllerCreationFunc> _entityPhysicsControllerCreationFunctionMap;
    std::map<std::string, EntityRenderControllerCreationFunc> _entityRenderControllerCreationFunctionMap;
    
    EntityManager();
    ~EntityManager() {}
    EntityManager(const EntityManager&);
    EntityManager& operator=(const EntityManager&);
};
