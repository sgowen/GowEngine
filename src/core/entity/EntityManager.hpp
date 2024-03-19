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

class EntityController;
class EntityControllerAI;
class EntityControllerInput;
class EntityControllerNetwork;
class EntityControllerRender;

typedef EntityController* (*EntityControllerCreationFunc)(Entity* e);
typedef EntityControllerAI* (*EntityControllerAICreationFunc)(Entity* e);
typedef EntityControllerInput* (*EntityControllerInputCreationFunc)(Entity* e);
typedef EntityControllerNetwork* (*EntityControllerNetworkCreationFunc)(Entity* e);
typedef EntityControllerRender* (*EntityControllerRenderCreationFunc)(Entity* e);

class EntityManager
{    
public:
    static EntityManager& getInstance()
    {
        static EntityManager ret = EntityManager();
        return ret;
    }
    
    void registerController(std::string name, EntityControllerCreationFunc func);
    EntityController* createEntityController(const EntityDef& ed, Entity* e);
    const std::map<std::string, EntityControllerCreationFunc>& getEntityControllerMap();
    
    void registerAIController(std::string name, EntityControllerAICreationFunc func);
    EntityControllerAI* createEntityControllerAI(const EntityDef& ed, Entity* e);
    const std::map<std::string, EntityControllerAICreationFunc>& getEntityControllerAIMap();
    
    void registerInputController(std::string name, EntityControllerInputCreationFunc func);
    EntityControllerInput* createEntityControllerInput(const EntityDef& ed, Entity* e);
    const std::map<std::string, EntityControllerInputCreationFunc>& getEntityControllerInputMap();
    
    void registerNetworkController(std::string name, EntityControllerNetworkCreationFunc func);
    EntityControllerNetwork* createEntityControllerNetwork(const EntityDef& ed, Entity* e);
    const std::map<std::string, EntityControllerNetworkCreationFunc>& getEntityControllerNetworkMap();
    
    void registerRenderController(std::string name, EntityControllerRenderCreationFunc func);
    EntityControllerRender* createEntityControllerRender(const EntityDef& ed, Entity* e);
    const std::map<std::string, EntityControllerRenderCreationFunc>& getEntityControllerRenderMap();
    
private:
    std::map<std::string, EntityControllerCreationFunc> _entityControllerCreationFunctions;
    std::map<std::string, EntityControllerAICreationFunc> _entityAIControllerCreationFunctions;
    std::map<std::string, EntityControllerInputCreationFunc> _entityInputControllerCreationFunctions;
    std::map<std::string, EntityControllerNetworkCreationFunc> _entityNetworkControllerCreationFunctions;
    std::map<std::string, EntityControllerRenderCreationFunc> _entityRenderControllerCreationFunctions;
    
    EntityManager();
    ~EntityManager() {}
    EntityManager(const EntityManager&);
    EntityManager& operator=(const EntityManager&);
};
