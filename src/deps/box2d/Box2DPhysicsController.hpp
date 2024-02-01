//
//  Box2DPhysicsController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
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
    
public:
    Box2DPhysicsController(Entity* e, b2World& world);
    virtual ~Box2DPhysicsController();
    
    virtual Vector2 velocity();
    virtual void setVelocity(Vector2 v);
    virtual void updatePoseFromBody();
    virtual void updateBodyFromPose() override;
    
    bool shouldCollide(Entity* e, b2Fixture* fixtureA, b2Fixture* fixtureB);
    void handleBeginContact(Entity* e, b2Fixture* fixtureA, b2Fixture* fixtureB);
    void handleEndContact(Entity* e, b2Fixture* fixtureA, b2Fixture* fixtureB);
    b2Body* getBody();
    
private:
    b2Body* _body;
    std::vector<b2Fixture*> _fixtures;
    b2Fixture* _groundSensorFixture;
    uint8_t _numGroundContacts;
    bool _isBodyFacingLeft;
    
    void createFixtures();
    void destroyFixtures();
};
