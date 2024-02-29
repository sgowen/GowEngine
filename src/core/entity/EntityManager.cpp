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
    EntityControllerCreationFunc func = _entityControllerCreationFunctions[ed._controller];
    assert(func != nullptr);
    return func(e);
}

const std::map<std::string, EntityControllerCreationFunc>& EntityManager::getEntityControllerMap()
{
    return _entityControllerCreationFunctions;
}

void EntityManager::registerNetworkController(std::string name, EntityNetworkControllerCreationFunc func)
{
    assert(func != nullptr);
    _entityNetworkControllerCreationFunctions[name] = func;
}

EntityNetworkController* EntityManager::createEntityNetworkController(EntityDef& ed, Entity* e)
{
    EntityNetworkControllerCreationFunc func = _entityNetworkControllerCreationFunctions[ed._networkController];
    assert(func != nullptr);
    return func(e);
}

const std::map<std::string, EntityNetworkControllerCreationFunc>& EntityManager::getEntityNetworkControllerMap()
{
    return _entityNetworkControllerCreationFunctions;
}

void EntityManager::registerRenderController(std::string name, EntityRenderControllerCreationFunc func)
{
    assert(func != nullptr);
    _entityRenderControllerCreationFunctions[name] = func;
}

EntityRenderController* EntityManager::createEntityRenderController(EntityDef& ed, Entity* e)
{
    EntityRenderControllerCreationFunc func = _entityRenderControllerCreationFunctions[ed._renderController];
    assert(func != nullptr);
    return func(e);
}

const std::map<std::string, EntityRenderControllerCreationFunc>& EntityManager::getEntityRenderControllerMap()
{
    return _entityRenderControllerCreationFunctions;
}

EntityManager::EntityManager()
{
    registerController("Default", EntityController::create);
    registerController("JonController", JonController::create);
    registerController("MonsterController", MonsterController::create);
    registerController("RobotController", RobotController::create);
    registerNetworkController("Default", EntityNetworkController::create);
    registerRenderController("Default", EntityRenderController::create);
}
