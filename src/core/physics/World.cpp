//
//  World.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI_NOPARENT(World)

World::World() :
_entityLayout(),
_numMovesProcessed(0)
{
    // Empty
}

World::~World()
{
    _entityLayout = EntityLayoutDef();
}

void World::populateFromEntityLayout(EntityLayoutDef& eld)
{
    reset();
    
    _entityLayout = eld;
    
    for (auto& eid : _entityLayout._entities)
    {
        addEntity(ENTITY_MGR.createEntity(eid));
    }
}

void World::addNetworkEntity(Entity* e)
{
    assert(!e->isLayer() && !e->isStatic());
    
    e->setWorld(this);
    e->setPhysicsController(createPhysicsController(e));
    
    if (e->isPlayer())
    {
        _players.push_back(e);
    }
    else if (e->isDynamic())
    {
        _networkEntities.push_back(e);
    }
}

void World::removeNetworkEntity(Entity* e)
{
    assert(!e->isLayer() && !e->isStatic());
    
    if (e->isPlayer())
    {
        removeEntity(e, _players);
    }
    else if (e->isDynamic())
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
        e->networkController()->storeToCache(_numMovesProcessed);
    }
    
    for (Entity* e : _networkEntities)
    {
        e->networkController()->storeToCache(_numMovesProcessed);
    }
}

void World::recallCache(uint32_t numMovesProcessed)
{
    _numMovesProcessed = numMovesProcessed;
    
    for (Entity* e : _players)
    {
        e->networkController()->recallCache(numMovesProcessed);
        e->physicsController()->updateBodyFromPose();
    }
    
    for (Entity* e : _networkEntities)
    {
        e->networkController()->recallCache(numMovesProcessed);
        e->physicsController()->updateBodyFromPose();
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
        e->update();
        if (e->isRequestingDeletion())
        {
            toDelete.push_back(e);
        }
    }
    
    for (Entity* e : _networkEntities)
    {
        e->update();
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
    _entityLayout = EntityLayoutDef();
    
    removeAllEntities(_layers);
    removeAllEntities(_staticEntities);
}

bool World::isEntityLayoutLoaded()
{
    return _entityLayout._key > 0;
}

EntityLayoutDef& World::getEntityLayout()
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

std::vector<Entity*>& World::getNetworkEntities()
{
    return _networkEntities;
}

std::vector<Entity*>& World::getPlayers()
{
    return _players;
}

uint32_t World::getNumMovesProcessed()
{
    return _numMovesProcessed;
}

void World::resetNumMovesProcessed()
{
    _numMovesProcessed = 0;
}

void World::addEntity(Entity *e)
{
    assert(!e->isPlayer() && !e->isDynamic());
    
    e->setWorld(this);
    
    if (e->isLayer())
    {
        _layers.push_back(e);
    }
    else if (e->isStatic())
    {
        e->setPhysicsController(createPhysicsController(e));
        
        _staticEntities.push_back(e);
    }
}

void World::removeEntity(Entity* e)
{
    assert(!e->isDynamic());
    
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
