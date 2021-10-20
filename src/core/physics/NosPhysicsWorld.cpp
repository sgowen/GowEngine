//
//  NosPhysicsWorld.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
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
    // TODO
}

EntityPhysicsController* NosPhysicsWorld::createPhysicsController(Entity* e)
{
    return new NosPhysicsController(e);
}
