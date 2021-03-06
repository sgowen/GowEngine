//
//  Box2DPhysicsController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/entity/EntityPhysicsController.hpp"

#include <vector>

class Entity;
class b2Body;
class b2Fixture;
class b2World;

class Box2DPhysicsController : public EntityPhysicsController
{
    DECL_RTTI;
    DECL_EntityController_create(EntityPhysicsController);
    
public:
    Box2DPhysicsController(Entity* e);
    virtual ~Box2DPhysicsController() {}
    
    virtual void updatePoseFromBody() override;
    virtual void updateBodyFromPose() override;
    
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
    float _bodyWidth;
    float _bodyHeight;
    bool _isBodyFacingLeft;
    
    void createFixtures();
    void destroyFixtures();
};
