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

struct Bounds
{
    uint32_t _flags;
    float _centerX;
    float _centerY;
    float _halfWidth;
    float _halfHeight;
    Rektangle _boundingBox;
    
    Bounds(uint32_t flags, float centerX, float centerY, float halfWidth, float halfHeight) :
    _flags(flags),
    _centerX(centerX),
    _centerY(centerY),
    _halfWidth(halfWidth),
    _halfHeight(halfHeight),
    _boundingBox(0, 0, halfWidth * 2, halfHeight * 2)
    {
        // Empty
    }
    
    void updateForPosition(Vector2 pos)
    {
        float l = _centerX - _halfWidth;
        float b = _centerY - _halfHeight;
        _boundingBox._lowerLeft.set(pos._x + l, pos._y + b);
    }
};

class Entity;

class NosPhysicsController : public EntityPhysicsController
{
    DECL_RTTI;
    
public:
    NosPhysicsController(Entity* e, float gravity);
    virtual ~NosPhysicsController();
    
    virtual Vector2 velocity() override;
    virtual void setVelocity(Vector2 v) override;
    virtual void updatePoseFromBody() override;
    virtual void updateBodyFromPose() override;
    
    void step(float deltaTime);
    void processCollisions(std::vector<Entity*>& entities);
    std::vector<Bounds>& bounds();
    
private:
    std::vector<Bounds> _bounds;
    Vector2 _velocity;
    Vector2 _position;
    float _gravity;
    float _tolerance;
    bool _isGrounded;
    bool _isBodyFacingLeft;
    
    bool crossesBottomEdge(float yourBottom, float myTop, float tolerance = 1.0f);
    bool crossesTopEdge(float yourTop, float myBottom, float tolerance = 1.0f);
    bool isInside(float yourBottom, float yourTop, float myBottom, float myTop);
    void createFixtures();
    void destroyFixtures();
};
