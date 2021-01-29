//
//  EntityController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/5/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#define DECL_EntityController_create \
public: \
    static EntityController* create(Entity* e)

#define IMPL_EntityController_create(name) \
EntityController* name::create(Entity* e)  \
{                                          \
    return new name(e);                    \
}                                          \

#include <vector>
#include <string>

#include "RTTI.hpp"

class Entity;
class b2Fixture;

class EntityController
{
    DECL_RTTI;
    DECL_EntityController_create;
    
public:
    EntityController(Entity* e);
    virtual ~EntityController();
    
    virtual void update();
    virtual void receiveMessage(uint16_t message, void* data = NULL);
    virtual void onFixturesCreated(std::vector<b2Fixture*>& fixtures);
    virtual bool shouldCollide(Entity* e, b2Fixture* fixtureA, b2Fixture* fixtureB);
    virtual void handleBeginContact(Entity* e, b2Fixture* fixtureA, b2Fixture* fixtureB);
    virtual void handleEndContact(Entity* e, b2Fixture* fixtureA, b2Fixture* fixtureB);
    
protected:
    Entity* _entity;
};
