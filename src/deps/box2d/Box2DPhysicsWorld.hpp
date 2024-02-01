//
//  Box2DPhysicsWorld.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/physics/World.hpp"

class b2World;
class EntityContactListener;
class EntityContactFilter;

class Box2DPhysicsWorld : public World
{
    DECL_RTTI;
    
public:    
    Box2DPhysicsWorld();
    virtual ~Box2DPhysicsWorld();
    
    virtual void stepPhysics(float deltaTime) override;
    virtual void extrapolatePhysics(float extrapolation) override;
    virtual void endExtrapolatedPhysics() override;
    
    b2World& getB2World();
    
protected:
    virtual EntityPhysicsController* createPhysicsController(Entity* e) override;
    
private:
    b2World* _world;
    EntityContactListener* _entityContactListener;
    EntityContactFilter* _entityContactFilter;
};

#include <box2d/b2_world_callbacks.h>

class b2Contact;

class EntityContactListener : public b2ContactListener
{
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
};

class b2Fixture;

class EntityContactFilter : public b2ContactFilter
{
    virtual bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
};
