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
    float _centerX;
    float _centerY;
    float _halfWidth;
    float _halfHeight;
    Rektangle _boundingBox;
    Line _left;
    Line _right;
    Line _bottom;
    Line _top;
    
    Bounds(float centerX, float centerY, float halfWidth, float halfHeight) :
    _centerX(centerX),
    _centerY(centerY),
    _halfWidth(halfWidth),
    _halfHeight(halfHeight),
    _boundingBox(0, 0, halfWidth * 2, halfHeight * 2),
    _left(),
    _right(),
    _bottom(),
    _top()
    {
        // Empty
    }
    
    void update()
    {
        float l = _boundingBox.left();
        float r = _boundingBox.right();
        float b = _boundingBox.bottom();
        float t = _boundingBox.top();
        
        _left._origin.set(l, b + 0.1f);
        _left._end.set(l, t - 0.1f);
        _right._origin.set(r, b + 0.1f);
        _right._end.set(r, t - 0.1f);
        _bottom._origin.set(l + 0.1f, b);
        _bottom._end.set(r - 0.1f, b);
        _top._origin.set(l + 0.1f, t);
        _top._end.set(r - 0.1f, t);
    }
    
    void updateForPosition(Vector2 pos)
    {
        float l = _centerX - _halfWidth;
        float b = _centerY - _halfHeight;
        _boundingBox._lowerLeft.set(pos._x + l, pos._y + b);
//        update();
    }
};

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
    std::vector<Bounds>& bounds();
    
private:
    std::vector<Bounds> _bounds;
    Vector2 _velocity;
    Vector2 _position;
    Bounds* _groundSensor;
    uint8_t _numGroundContacts;
    bool _isBodyFacingLeft;
    
    void createFixtures();
    void destroyFixtures();
};
