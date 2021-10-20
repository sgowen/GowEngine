//
//  NosPhysicsController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/entity/EntityPhysicsController.hpp"

#include "core/math/Rektangle.hpp"

#include <vector>

class Entity;

class NosPhysicsController : public EntityPhysicsController
{
    DECL_RTTI;
    
public:
    NosPhysicsController(Entity* e);
    virtual ~NosPhysicsController();
    
    virtual Vector2 velocity() override;
    virtual void setVelocity(Vector2 v) override;
    virtual void updatePoseFromBody() override;
    virtual void updateBodyFromPose() override;
    
    void step(float gravity, float deltaTime);
    void processCollisions(std::vector<Entity*>& entities);
    
private:
    std::vector<Rektangle> _boundingBoxes;
    Vector2 _velocity;
    Vector2 _position;
    Rektangle* _groundSensor;
    uint8_t _numGroundContacts;
    bool _isBodyFacingLeft;
    
    void createFixtures();
    void destroyFixtures();
};
