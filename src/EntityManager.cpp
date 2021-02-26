//
//  EntityManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/5/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "EntityManager.hpp"

#include "Entity.hpp"
#include "EntityLayoutManager.hpp"
#include "EntityController.hpp"
#include "EntityNetworkController.hpp"
#include "EntityPhysicsController.hpp"
#include "EntityRenderController.hpp"
#include "EntityController.hpp"
#include "StringUtil.hpp"
#include "STLUtil.hpp"
#include "AssetHandlerFactory.hpp"
#include "AssetHandler.hpp"
#include "FileData.hpp"

#include <assert.h>

Entity* EntityManager::createEntity(EntityInstanceDef eid)
{
    EntityDef& ed = getEntityDef(eid._key);
    return new Entity(ed, eid);
}

EntityDef& EntityManager::getEntityDef(uint32_t fourCCName)
{
    auto q = _entityDescriptorsMap.find(fourCCName);
    assert(q != _entityDescriptorsMap.end());
    return q->second;
}

void EntityManager::registerController(std::string name, EntityControllerCreationFunc func)
{
    assert(func != NULL);
    _entityControllerCreationFunctionMap[name] = func;
}

EntityController* EntityManager::createEntityController(EntityDef& ed, Entity* e)
{
    EntityControllerCreationFunc func = _entityControllerCreationFunctionMap[ed._controller];
    assert(func != NULL);
    return func(e);
}

const std::map<std::string, EntityControllerCreationFunc>& EntityManager::getEntityControllerMap()
{
    return _entityControllerCreationFunctionMap;
}

void EntityManager::registerNetworkController(std::string name, EntityNetworkControllerCreationFunc func)
{
    assert(func != NULL);
    _entityNetworkControllerCreationFunctionMap[name] = func;
}

EntityNetworkController* EntityManager::createEntityNetworkController(EntityDef& ed, Entity* e)
{
    EntityNetworkControllerCreationFunc func = _entityNetworkControllerCreationFunctionMap[ed._networkController];
    assert(func != NULL);
    return func(e);
}

const std::map<std::string, EntityNetworkControllerCreationFunc>& EntityManager::getEntityNetworkControllerMap()
{
    return _entityNetworkControllerCreationFunctionMap;
}

void EntityManager::registerPhysicsController(std::string name, EntityPhysicsControllerCreationFunc func)
{
    assert(func != NULL);
    _entityPhysicsControllerCreationFunctionMap[name] = func;
}

EntityPhysicsController* EntityManager::createEntityPhysicsController(EntityDef& ed, Entity* e)
{
    EntityPhysicsControllerCreationFunc func = _entityPhysicsControllerCreationFunctionMap[ed._physicsController];
    assert(func != NULL);
    return func(e);
}

const std::map<std::string, EntityPhysicsControllerCreationFunc>& EntityManager::getEntityPhysicsControllerMap()
{
    return _entityPhysicsControllerCreationFunctionMap;
}

void EntityManager::registerRenderController(std::string name, EntityRenderControllerCreationFunc func)
{
    assert(func != NULL);
    _entityRenderControllerCreationFunctionMap[name] = func;
}

EntityRenderController* EntityManager::createEntityRenderController(EntityDef& ed, Entity* e)
{
    EntityRenderControllerCreationFunc func = _entityRenderControllerCreationFunctionMap[ed._renderController];
    assert(func != NULL);
    return func(e);
}

const std::map<std::string, EntityRenderControllerCreationFunc>& EntityManager::getEntityRenderControllerMap()
{
    return _entityRenderControllerCreationFunctionMap;
}

EntityManager::EntityManager()
{
    registerController("Default", EntityController::create);
    registerNetworkController("Default", EntityNetworkController::create);
    registerPhysicsController("Default", EntityPhysicsController::create);
    registerRenderController("Default", EntityRenderController::create);
}
