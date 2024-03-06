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
    // Empty
}

void NosPhysicsWorld::stepPhysics(float deltaTime)
{
    for (Entity* e : _players)
    {
        NosControllerPhysics* epc = e->controllerPhysics<NosControllerPhysics>();
        epc->step(deltaTime);
    }
    
    for (Entity* e : _networkEntities)
    {
        NosControllerPhysics* epc = e->controllerPhysics<NosControllerPhysics>();
        epc->step(deltaTime);
    }
    
    for (Entity* e : _players)
    {
        NosControllerPhysics* epc = e->controllerPhysics<NosControllerPhysics>();
        epc->processCollisions(_networkEntities);
        epc->processCollisions(_staticEntities);
    }
    
    for (Entity* e : _networkEntities)
    {
        NosControllerPhysics* epc = e->controllerPhysics<NosControllerPhysics>();
        epc->processCollisions(_players);
        epc->processCollisions(_networkEntities);
        epc->processCollisions(_staticEntities);
    }
}

void NosPhysicsWorld::extrapolatePhysics(float extrapolation)
{
    for (Entity* e : _players)
    {
        NosControllerPhysics* epc = e->controllerPhysics<NosControllerPhysics>();
        epc->extrapolate(extrapolation);
    }
    
    for (Entity* e : _networkEntities)
    {
        NosControllerPhysics* epc = e->controllerPhysics<NosControllerPhysics>();
        epc->extrapolate(extrapolation);
    }
}

void NosPhysicsWorld::endExtrapolatedPhysics()
{
    for (Entity* e : _players)
    {
        NosControllerPhysics* epc = e->controllerPhysics<NosControllerPhysics>();
        epc->endExtrapolation();
    }
    
    for (Entity* e : _networkEntities)
    {
        NosControllerPhysics* epc = e->controllerPhysics<NosControllerPhysics>();
        epc->endExtrapolation();
    }
}

EntityControllerPhysics* NosPhysicsWorld::createControllerPhysics(Entity* e)
{
    return new NosControllerPhysics(e);
}
