//
//  Box2DPhysicsController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "EntityPhysicsController.hpp"

#include <box2d/box2d.h>

#include <vector>

class Entity;

class Box2DPhysicsController : public EntityPhysicsController
{
    DECL_EntityController_create(EntityPhysicsController);
    
public:
    Box2DPhysicsController(Entity* e);
    virtual ~Box2DPhysicsController() {}
    
    virtual void updatePoseFromBody();
    virtual void updateBodyFromPose();
    
    bool shouldCollide(Entity* e, b2Fixture* fixtureA, b2Fixture* fixtureB);
    void handleBeginContact(Entity* e, b2Fixture* fixtureA, b2Fixture* fixtureB);
    void handleEndContact(Entity* e, b2Fixture* fixtureA, b2Fixture* fixtureB);
    void initPhysics(b2World& world);
    void deinitPhysics();
    b2Body* getBody();
    
protected:
    b2Body* _body;
    std::vector<b2Fixture*> _fixtures;
    b2Fixture* _groundSensorFixture;
    int _bodyWidth;
    int _bodyHeight;
    bool _isBodyFacingLeft;
    
    void createFixtures();
    void destroyFixtures();
};
