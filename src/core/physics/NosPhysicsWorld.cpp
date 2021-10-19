//
//  NosPhysicsWorld.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

bool NosPhysicsWorld::s_isClient = false;
bool NosPhysicsWorld::s_isLiveFrame = false;

NosPhysicsWorld::NosPhysicsWorld(TimeTracker& tt) :
_timeTracker(tt),
_entityLayout()
{
    // Empty
}

NosPhysicsWorld::~NosPhysicsWorld()
{
    reset();
}

void NosPhysicsWorld::populateFromEntityLayout(EntityLayoutDef& eld)
{
    reset();
    
    _entityLayout = eld;
    
    for (auto& eid : _entityLayout._entities)
    {
        addEntity(ENTITY_MGR.createEntity(eid));
    }
}

void NosPhysicsWorld::addNetworkEntity(Entity* e)
{
    assert(!e->isLayer() && !e->isStatic());
    
    NosPhysicsController* epc = e->physicsController<NosPhysicsController>();
//    epc->initPhysics(*_world);
    
    if (e->isPlayer())
    {
        _players.push_back(e);
    }
    else if (e->isDynamic())
    {
        _networkEntities.push_back(e);
    }
}

void NosPhysicsWorld::removeNetworkEntity(Entity* e)
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

void NosPhysicsWorld::recallCache()
{
    for (Entity* e : _players)
    {
        e->networkController()->recallCache();
    }
    
    for (Entity* e : _networkEntities)
    {
        e->networkController()->recallCache();
    }
}

void NosPhysicsWorld::stepPhysics()
{
    // TODO
}

std::vector<Entity*> NosPhysicsWorld::update()
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
    
    return toDelete;
}

void NosPhysicsWorld::reset()
{
    _entityLayout = EntityLayoutDef();
    
    removeAllEntities(_layers);
    removeAllEntities(_staticEntities);
}

bool NosPhysicsWorld::isEntityLayoutLoaded()
{
    return _entityLayout._key > 0;
}

EntityLayoutDef& NosPhysicsWorld::getEntityLayout()
{
    return _entityLayout;
}

std::vector<Entity*>& NosPhysicsWorld::getLayers()
{
    return _layers;
}

std::vector<Entity*>& NosPhysicsWorld::getStaticEntities()
{
    return _staticEntities;
}

std::vector<Entity*>& NosPhysicsWorld::getNetworkEntities()
{
    return _networkEntities;
}

std::vector<Entity*>& NosPhysicsWorld::getPlayers()
{
    return _players;
}

void NosPhysicsWorld::addEntity(Entity *e)
{
    assert(!e->isPlayer() && !e->isDynamic());
    
    if (e->isLayer())
    {
        _layers.push_back(e);
    }
    else if (e->isStatic())
    {
        NosPhysicsController* epc = e->physicsController<NosPhysicsController>();
//        epc->initPhysics(*_world);
        
        _staticEntities.push_back(e);
    }
}

void NosPhysicsWorld::removeEntity(Entity* e)
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

void NosPhysicsWorld::removeEntity(Entity* e, std::vector<Entity*>& entities)
{
    assert(e != nullptr);
    
    for (std::vector<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
    {
        Entity* entityToDelete = *i;
        if (entityToDelete == e)
        {
            if (!entityToDelete->isLayer())
            {
                NosPhysicsController* epc = entityToDelete->physicsController<NosPhysicsController>();
//                epc->deinitPhysics();
            }
            
            delete entityToDelete;
            
            entities.erase(i);
            return;
        }
    }
}

void NosPhysicsWorld::removeAllEntities(std::vector<Entity*>& entities)
{
    for (std::vector<Entity*>::iterator i = entities.begin(); i != entities.end(); )
    {
        Entity* entityToDelete = *i;
        if (!entityToDelete->isLayer())
        {
            NosPhysicsController* epc = entityToDelete->physicsController<NosPhysicsController>();
//            epc->deinitPhysics();
        }
        
        delete entityToDelete;
        
        i = entities.erase(i);
    }
}
