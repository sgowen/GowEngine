//
//  EntityManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/5/18.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Entity.hpp"

#include <map>
#include <vector>
#include <string>

struct EntityInstanceDef;
struct EntityDef;
class EntityController;
class EntityNetworkController;
class EntityPhysicsController;
class EntityRenderController;

typedef EntityController* (*EntityControllerCreationFunc)(Entity* e);
typedef EntityNetworkController* (*EntityNetworkControllerCreationFunc)(Entity* e);
typedef EntityRenderController* (*EntityRenderControllerCreationFunc)(Entity* e);

class EntityManager
{
    friend class EntityManagerLoader;
    
public:
    static EntityManager& getInstance()
    {
        static EntityManager ret = EntityManager();
        return ret;
    }
    
    Entity* createEntity(EntityInstanceDef eid);
    EntityDef& getEntityDef(uint32_t fourCCName);
    Entity* createEntity(EntityDef& ed, EntityInstanceDef eid);
    
    void registerController(std::string name, EntityControllerCreationFunc func);
    EntityController* createEntityController(EntityDef& ed, Entity* e);
    const std::map<std::string, EntityControllerCreationFunc>& getEntityControllerMap();
    
    void registerNetworkController(std::string name, EntityNetworkControllerCreationFunc func);
    EntityNetworkController* createEntityNetworkController(EntityDef& ed, Entity* e);
    const std::map<std::string, EntityNetworkControllerCreationFunc>& getEntityNetworkControllerMap();
    
    void registerRenderController(std::string name, EntityRenderControllerCreationFunc func);
    EntityRenderController* createEntityRenderController(EntityDef& ed, Entity* e);
    const std::map<std::string, EntityRenderControllerCreationFunc>& getEntityRenderControllerMap();
    
private:
    std::map<uint32_t, EntityDef> _entityDefs;
    std::map<std::string, EntityControllerCreationFunc> _entityControllerCreationFunctions;
    std::map<std::string, EntityNetworkControllerCreationFunc> _entityNetworkControllerCreationFunctions;
    std::map<std::string, EntityRenderControllerCreationFunc> _entityRenderControllerCreationFunctions;
    
    EntityManager();
    ~EntityManager() {}
    EntityManager(const EntityManager&);
    EntityManager& operator=(const EntityManager&);
};
