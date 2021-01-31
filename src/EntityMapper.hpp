//
//  EntityMapper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/5/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <map>
#include <vector>
#include <string>

class Entity;
struct EntityDef;
struct EntityInstanceDef;
class EntityController;
class EntityNetworkController;

typedef EntityController* (*EntityControllerCreationFunc)(Entity* e);
typedef EntityNetworkController* (*EntityNetworkControllerCreationFunc)(Entity* e, bool isServer);

#define ENTITY_MAPPER EntityMapper::getInstance()

class EntityMapper
{
public:
    static EntityMapper& getInstance();
    
    void initWithJSONFile(const char* filePath);
    void initWithJSON(const char* json);
    void clear();
    Entity* createEntity(EntityInstanceDef* eid, bool isServer);
    Entity* createEntityFromDef(EntityDef* entityDef, EntityInstanceDef* eid, bool isServer);
    EntityDef* getEntityDef(uint32_t fourCCName);
    void registerFunction(std::string name, EntityControllerCreationFunc func);
    void registerFunction(std::string name, EntityNetworkControllerCreationFunc func);
    EntityController* createEntityController(EntityDef& ed, Entity* e);
    EntityNetworkController* createEntityNetworkController(EntityDef& ed, Entity* e);
    const std::vector<EntityDef*>& getEntityDescriptors();
    const std::map<std::string, EntityControllerCreationFunc>& getEntityControllerMap();
    const std::map<std::string, EntityNetworkControllerCreationFunc>& getEntityNetworkControllerMap();
    
private:
    std::map<uint32_t, EntityDef*> _entityDescriptorsMap;
    std::vector<EntityDef*> _entityDescriptors;
    std::map<std::string, EntityControllerCreationFunc> _entityControllerCreationFunctionMap;
    std::map<std::string, EntityNetworkControllerCreationFunc> _entityNetworkControllerCreationFunctionMap;
    
    EntityMapper();
    ~EntityMapper();
    EntityMapper(const EntityMapper&);
    EntityMapper& operator=(const EntityMapper&);
};
