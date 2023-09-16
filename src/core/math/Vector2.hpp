//
//  Vector2.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "MathUtil.hpp"

class Vector2
{
public:
    static const Vector2 ZERO;
    
    float _x;
    float _y;
    
    friend Vector2 operator+(const Vector2& a, const Vector2& b);
    friend Vector2 operator-(const Vector2& a, const Vector2& b);
    friend Vector2 operator*(float scalar, const Vector2& other);
    friend Vector2 operator*(const Vector2& other, float scalar);
    friend Vector2 operator*(const Vector2& a, const Vector2& b);
    friend bool operator==(const Vector2& a, const Vector2& b);
    friend bool operator!=(const Vector2& a, const Vector2& b);
    friend Vector2 lerp(Vector2& left, Vector2& right, float t);
    
    Vector2(float x = 0, float y = 0);
    Vector2(const Vector2& other);
    
    Vector2& set(const Vector2& other);
    Vector2& set(float x, float y);
    Vector2& setX(float x);
    Vector2& setY(float y);
    void operator+=(const Vector2& other);
    Vector2& add(float x, float y);
    void operator-=(const Vector2& other);
    Vector2& sub(float x, float y);
    Vector2 operator*=(float scalar);
    Vector2& mul(float scalar);
    Vector2 operator/=(float scalar);
    Vector2& div(float scalar);
    Vector2 cpy();
    float len();
    float lenSquared();
    Vector2& nor();
    float angle();
    Vector2& rotate(float angle);
    float dot(const Vector2& other);
    float dist(const Vector2& other);
    float dist(float x, float y) const;
    float distSquared(const Vector2& other) const;
    float distSquared(float x, float y) const;
    void reset();
};
