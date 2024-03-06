//
//  Box2DControllerPhysics.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/entity/EntityControllerPhysics.hpp"

#include <vector>

class Entity;
class b2Body;
class b2Fixture;
class b2World;

class Box2DControllerPhysics : public EntityControllerPhysics
{
    DECL_RTTI;
    
public:
    Box2DControllerPhysics(Entity* e, b2World& world);
    virtual ~Box2DControllerPhysics();
    
    virtual void updateBodyFromPose() override;
    
    void updatePoseFromBody();
    bool shouldCollide(Entity* e, b2Fixture* fixtureA, b2Fixture* fixtureB);
    void handleBeginContact(Entity* e, b2Fixture* fixtureA, b2Fixture* fixtureB);
    void handleEndContact(Entity* e, b2Fixture* fixtureA, b2Fixture* fixtureB);
    b2Body* getBody();
    
private:
    b2Body* _body;
    std::vector<b2Fixture*> _fixtures;
    b2Fixture* _groundSensorFixture;
    b2Fixture* _damageSensorFixture;
    uint8_t _numGroundContacts;
    bool _isBodyFacingLeft;
    
    void createFixtures();
    void destroyFixtures();
};
