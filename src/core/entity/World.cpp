//
//  World.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI_NOPARENT(World)

World::World() :
_entityLayout(),
_numMovesProcessed(0),
_rightEdge(0),
_topEdge(0)
{
    // Empty
}

World::~World()
{
    reset();
    removeAllNetworkEntities();
}

void World::populateFromEntityLayout(EntityLayout& el)
{
    reset();
    
    _entityLayout = el;
    
    for (auto& eid : _entityLayout._entities)
    {
        EntityDef& ed = ASSETS_MGR.getEntityDef(eid._key);
        Entity* e = Entity::createEntity(ed, eid);
        addEntity(e);
        
        uint32_t rightEdge = eid._x + ed._width;
        if (rightEdge > _rightEdge)
        {
            _rightEdge = rightEdge;
        }
        
        uint32_t topEdge = eid._y + ed._height;
        if (topEdge > _topEdge)
        {
            _topEdge = topEdge;
        }
    }
}

void World::addNetworkEntity(Entity* e)
{
    assert(e->isPlayer() || e->isStateSensitive());
    
    e->setWorld(this);
    e->setControllerPhysics(createControllerPhysics(e));
    
    if (e->isPlayer())
    {
        _players.push_back(e);
    }
    else if (e->isStateSensitive())
    {
        _networkEntities.push_back(e);
    }
}

void World::removeNetworkEntity(Entity* e)
{
    assert(e->isPlayer() || e->isStateSensitive());
    
    e->destroyControllerPhysics();
    
    if (e->isPlayer())
    {
        removeEntity(e, _players);
    }
    else if (e->isStateSensitive())
    {
        removeEntity(e, _networkEntities);
    }
}

void World::removeAllNetworkEntities()
{
    removeAllEntities(_players);
    removeAllEntities(_networkEntities);
}

void World::storeToCache()
{
    for (Entity* e : _players)
    {
        e->controllerNetwork()->storeToCache(_numMovesProcessed);
    }
    
    for (Entity* e : _networkEntities)
    {
        e->controllerNetwork()->storeToCache(_numMovesProcessed);
    }
}

void World::recallCache(uint32_t numMovesProcessed)
{
    _numMovesProcessed = numMovesProcessed;
    
    for (Entity* e : _players)
    {
        e->controllerNetwork()->recallCache(numMovesProcessed);
        e->controllerPhysics()->updateBodyFromPose();
    }
    
    for (Entity* e : _networkEntities)
    {
        e->controllerNetwork()->recallCache(numMovesProcessed);
        e->controllerPhysics()->updateBodyFromPose();
    }
}

void World::clearCache(uint32_t numMovesProcessed)
{
    for (Entity* e : _players)
    {
        e->controllerNetwork()->clearCache(numMovesProcessed);
    }
    
    for (Entity* e : _networkEntities)
    {
        e->controllerNetwork()->clearCache(numMovesProcessed);
    }
}

void World::beginFrame()
{
    for (Entity* e : _players)
    {
        e->beginFrame();
    }
    
    for (Entity* e : _networkEntities)
    {
        e->beginFrame();
    }
}

std::vector<Entity*> World::update()
{
    std::vector<Entity*> toDelete;
    for (Entity* e : _players)
    {
        e->update(_numMovesProcessed);
        if (e->isRequestingDeletion())
        {
            toDelete.push_back(e);
        }
    }
    
    for (Entity* e : _networkEntities)
    {
        e->update(_numMovesProcessed);
        if (e->isRequestingDeletion())
        {
            toDelete.push_back(e);
        }
    }
    
    ++_numMovesProcessed;
    
    return toDelete;
}

void World::reset()
{
    _entityLayout = EntityLayout();
    _rightEdge = 0;
    _topEdge = 0;
    
    removeAllEntities(_layers);
    removeAllEntities(_staticEntities);
}

uint32_t World::rightEdge()
{
    return _rightEdge;
}

uint32_t World::topEdge()
{
    return _topEdge;
}

bool World::isEntityLayoutLoaded()
{
    return _entityLayout._key > 0;
}

EntityLayout& World::getEntityLayout()
{
    return _entityLayout;
}

std::vector<Entity*>& World::getLayers()
{
    return _layers;
}

std::vector<Entity*>& World::getStaticEntities()
{
    return _staticEntities;
}

std::vector<Entity*>& World::getDynamicEntities()
{
    return _networkEntities;
}

std::vector<Entity*>& World::getPlayers()
{
    return _players;
}

Entity* World::getPlayer(uint8_t playerID)
{
    Entity* ret = nullptr;
    
    for (Entity* e : getPlayers())
    {
        if (playerID == e->playerInfo()._playerID)
        {
            ret = e;
            break;
        }
    }
    
    return ret;
}

uint32_t World::getNumMovesProcessed()
{
    return _numMovesProcessed;
}

void World::resetNumMovesProcessed()
{
    _numMovesProcessed = 0;
}

Entity* World::getEntityByID(uint32_t id) const
{
    for (Entity* e : _players)
    {
        if (e->getID() == id)
        {
            return e;
        }
    }
    
    for (Entity* e : _networkEntities)
    {
        if (e->getID() == id)
        {
            return e;
        }
    }
    
    for (Entity* e : _staticEntities)
    {
        if (e->getID() == id)
        {
            return e;
        }
    }
    
    for (Entity* e : _layers)
    {
        if (e->getID() == id)
        {
            return e;
        }
    }
    
    return nullptr;
}

void World::addEntity(Entity *e)
{
    assert(e->isLayer() || e->isStatic());
    
    e->setWorld(this);
    
    if (e->isLayer())
    {
        _layers.push_back(e);
    }
    else if (e->isStatic())
    {
        e->setControllerPhysics(createControllerPhysics(e));
        
        _staticEntities.push_back(e);
    }
}

void World::removeEntity(Entity* e)
{
    assert(e->isLayer() || e->isStatic());
    
    if (e->isLayer())
    {
        removeEntity(e, _layers);
    }
    else if (e->isStatic())
    {
        removeEntity(e, _staticEntities);
    }
}

void World::removeEntity(Entity* e, std::vector<Entity*>& entities)
{
    assert(e != nullptr);
    
    for (std::vector<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
    {
        Entity* entityToDelete = *i;
        if (entityToDelete == e)
        {
            delete entityToDelete;
            entities.erase(i);
            return;
        }
    }
}

void World::removeAllEntities(std::vector<Entity*>& entities)
{
    for (std::vector<Entity*>::iterator i = entities.begin(); i != entities.end(); )
    {
        Entity* entityToDelete = *i;
        delete entityToDelete;
        i = entities.erase(i);
    }
}
