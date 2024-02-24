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

void World::populateFromEntityLayout(EntityLayoutDef& eld)
{
    reset();
    
    _entityLayout = eld;
    
    for (auto& eid : _entityLayout._entities)
    {
        Entity* e = ENTITY_MGR.createEntity(eid);
        addEntity(e);
        
        EntityDef& ed = ENTITY_MGR.getEntityDef(eid._key);
        
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
    assert(!e->isLayer() && !e->isStatic());
    
    e->setWorld(this);
    e->setPhysicsController(createPhysicsController(e));
    
    if (e->isPlayer())
    {
        _players.push_back(e);
    }
    else if (e->isDynamic())
    {
        _dynamicEntities.push_back(e);
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
        removeEntity(e, _dynamicEntities);
    }
}

void World::removeAllNetworkEntities()
{
    removeAllEntities(_players);
    removeAllEntities(_dynamicEntities);
}

void World::storeToCache()
{
    for (Entity* e : _players)
    {
        e->networkController()->storeToCache(_numMovesProcessed);
    }
    
    for (Entity* e : _dynamicEntities)
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
    
    for (Entity* e : _dynamicEntities)
    {
        e->networkController()->recallCache(numMovesProcessed);
        e->physicsController()->updateBodyFromPose();
    }
}

void World::clearCache(uint32_t numMovesProcessed)
{
    for (Entity* e : _players)
    {
        e->networkController()->clearCache(numMovesProcessed);
    }
    
    for (Entity* e : _dynamicEntities)
    {
        e->networkController()->clearCache(numMovesProcessed);
    }
}

void World::beginFrame()
{
    for (Entity* e : _players)
    {
        e->beginFrame();
    }
    
    for (Entity* e : _dynamicEntities)
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
    
    for (Entity* e : _dynamicEntities)
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
    _entityLayout = EntityLayoutDef();
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

std::vector<Entity*>& World::getDynamicEntities()
{
    return _dynamicEntities;
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
