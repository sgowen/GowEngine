//
//  NosPhysicsWorld.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI(NosPhysicsWorld, World)

NosPhysicsWorld::NosPhysicsWorld() : World()
{
    // Empty
}

NosPhysicsWorld::~NosPhysicsWorld()
{
    reset();
}

void NosPhysicsWorld::stepPhysics(float deltaTime)
{
    for (Entity* e : _players)
    {
        NosPhysicsController* epc = e->physicsController<NosPhysicsController>();
        epc->step(deltaTime);
    }
    
    for (Entity* e : _dynamicEntities)
    {
        NosPhysicsController* epc = e->physicsController<NosPhysicsController>();
        epc->step(deltaTime);
    }
    
    for (Entity* e : _players)
    {
        NosPhysicsController* epc = e->physicsController<NosPhysicsController>();
        epc->processCollisions(_dynamicEntities);
        epc->processCollisions(_staticEntities);
    }
    
    for (Entity* e : _dynamicEntities)
    {
        NosPhysicsController* epc = e->physicsController<NosPhysicsController>();
        epc->processCollisions(_players);
        epc->processCollisions(_dynamicEntities);
        epc->processCollisions(_staticEntities);
    }
}

void NosPhysicsWorld::extrapolatePhysics(float extrapolation)
{
    for (Entity* e : _players)
    {
        NosPhysicsController* epc = e->physicsController<NosPhysicsController>();
        epc->extrapolate(extrapolation);
    }
    
    for (Entity* e : _dynamicEntities)
    {
        NosPhysicsController* epc = e->physicsController<NosPhysicsController>();
        epc->extrapolate(extrapolation);
    }
}

void NosPhysicsWorld::endExtrapolatedPhysics()
{
    for (Entity* e : _players)
    {
        NosPhysicsController* epc = e->physicsController<NosPhysicsController>();
        epc->endExtrapolation();
    }
    
    for (Entity* e : _dynamicEntities)
    {
        NosPhysicsController* epc = e->physicsController<NosPhysicsController>();
        epc->endExtrapolation();
    }
}

EntityPhysicsController* NosPhysicsWorld::createPhysicsController(Entity* e)
{
    return new NosPhysicsController(e);
}
