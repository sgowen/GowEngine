//
//  NosPhysicsWorld.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI(NosPhysicsWorld, World)

NosPhysicsWorld::NosPhysicsWorld() : World(),
_gravity(-120.0f)
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
    
    for (Entity* e : _networkEntities)
    {
        NosPhysicsController* epc = e->physicsController<NosPhysicsController>();
        epc->step(deltaTime);
    }
    
    for (Entity* e : _players)
    {
        NosPhysicsController* epc = e->physicsController<NosPhysicsController>();
        epc->processCollisions(_networkEntities);
        epc->processCollisions(_staticEntities);
    }
    
    for (Entity* e : _networkEntities)
    {
        NosPhysicsController* epc = e->physicsController<NosPhysicsController>();
        epc->processCollisions(_players);
        epc->processCollisions(_networkEntities);
        epc->processCollisions(_staticEntities);
    }
}

void NosPhysicsWorld::interpolatePhysics(float interpolation)
{
    for (Entity* e : _players)
    {
        NosPhysicsController* epc = e->physicsController<NosPhysicsController>();
        epc->interpolate(interpolation);
    }
    
    for (Entity* e : _networkEntities)
    {
        NosPhysicsController* epc = e->physicsController<NosPhysicsController>();
        epc->interpolate(interpolation);
    }
}

void NosPhysicsWorld::endInterpolatedPhysics()
{
    for (Entity* e : _players)
    {
        NosPhysicsController* epc = e->physicsController<NosPhysicsController>();
        epc->endInterpolation();
    }
    
    for (Entity* e : _networkEntities)
    {
        NosPhysicsController* epc = e->physicsController<NosPhysicsController>();
        epc->endInterpolation();
    }
}

EntityPhysicsController* NosPhysicsWorld::createPhysicsController(Entity* e)
{
    return new NosPhysicsController(e, _gravity);
}
