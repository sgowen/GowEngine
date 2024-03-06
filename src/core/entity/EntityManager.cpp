//
//  EntityManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/5/18.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

Entity* EntityManager::createEntity(EntityInstanceDef eid)
{
    EntityDef& ed = getEntityDef(eid._key);
    return createEntity(ed, eid);
}

EntityDef& EntityManager::getEntityDef(uint32_t fourCCName)
{
    auto q = _entityDefs.find(fourCCName);
    assert(q != _entityDefs.end());
    return q->second;
}

Entity* EntityManager::createEntity(EntityDef& ed, EntityInstanceDef eid)
{
    return new Entity(ed, eid);
}

void EntityManager::registerController(std::string name, EntityControllerCreationFunc func)
{
    assert(func != nullptr);
    _entityControllerCreationFunctions[name] = func;
}

EntityController* EntityManager::createEntityController(EntityDef& ed, Entity* e)
{
    EntityControllerCreationFunc func;
    if (ed._controllerScript.empty())
    {
        func = _entityControllerCreationFunctions[ed._controller];
    }
    else
    {
        func = _entityControllerCreationFunctions["LuaController"];
    }
    assert(func != nullptr);
    
    return func(e);
}

const std::map<std::string, EntityControllerCreationFunc>& EntityManager::getEntityControllerMap()
{
    return _entityControllerCreationFunctions;
}

void EntityManager::registerAIController(std::string name, EntityControllerAICreationFunc func)
{
    assert(func != nullptr);
    _entityAIControllerCreationFunctions[name] = func;
}

EntityControllerAI* EntityManager::createEntityControllerAI(EntityDef& ed, Entity* e)
{
    EntityControllerAICreationFunc func;
    if (ed._controllerAIScript.empty())
    {
        func = _entityAIControllerCreationFunctions[ed._controllerAI];
    }
    else
    {
        func = _entityAIControllerCreationFunctions["LuaControllerAI"];
    }
    assert(func != nullptr);
    
    return func(e);
}

const std::map<std::string, EntityControllerAICreationFunc>& EntityManager::getEntityControllerAIMap()
{
    return _entityAIControllerCreationFunctions;
}

void EntityManager::registerInputController(std::string name, EntityControllerInputCreationFunc func)
{
    assert(func != nullptr);
    _entityInputControllerCreationFunctions[name] = func;
}

EntityControllerInput* EntityManager::createEntityControllerInput(EntityDef& ed, Entity* e)
{
    EntityControllerInputCreationFunc func = _entityInputControllerCreationFunctions[ed._controllerInput];
    assert(func != nullptr);
    return func(e);
}

const std::map<std::string, EntityControllerInputCreationFunc>& EntityManager::getEntityControllerInputMap()
{
    return _entityInputControllerCreationFunctions;
}

void EntityManager::registerNetworkController(std::string name, EntityControllerNetworkCreationFunc func)
{
    assert(func != nullptr);
    _entityNetworkControllerCreationFunctions[name] = func;
}

EntityControllerNetwork* EntityManager::createEntityControllerNetwork(EntityDef& ed, Entity* e)
{
    EntityControllerNetworkCreationFunc func = _entityNetworkControllerCreationFunctions[ed._controllerNetwork];
    assert(func != nullptr);
    return func(e);
}

const std::map<std::string, EntityControllerNetworkCreationFunc>& EntityManager::getEntityControllerNetworkMap()
{
    return _entityNetworkControllerCreationFunctions;
}

void EntityManager::registerRenderController(std::string name, EntityControllerRenderCreationFunc func)
{
    assert(func != nullptr);
    _entityRenderControllerCreationFunctions[name] = func;
}

EntityControllerRender* EntityManager::createEntityControllerRender(EntityDef& ed, Entity* e)
{
    EntityControllerRenderCreationFunc func = _entityRenderControllerCreationFunctions[ed._controllerRender];
    assert(func != nullptr);
    return func(e);
}

const std::map<std::string, EntityControllerRenderCreationFunc>& EntityManager::getEntityControllerRenderMap()
{
    return _entityRenderControllerCreationFunctions;
}

EntityManager::EntityManager()
{
    registerController("EntityController", EntityController::create);
    registerController("LuaController", LuaController::create);
    registerAIController("EntityControllerAI", EntityControllerAI::create);
    registerAIController("LuaControllerAI", LuaControllerAI::create);
    registerInputController("EntityControllerInput", EntityControllerInput::create);
    registerNetworkController("EntityControllerNetwork", EntityControllerNetwork::create);
    registerRenderController("EntityControllerRender", EntityControllerRender::create);
}
