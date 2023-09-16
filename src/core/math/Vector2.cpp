//
//  Vector2.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

const Vector2 Vector2::ZERO = Vector2(VECTOR2_ZERO);

Vector2 operator+(const Vector2& a, const Vector2& b)
{
    return Vector2(a._x + b._x, a._y + b._y);
}

Vector2 operator-(const Vector2& a, const Vector2& b)
{
    return Vector2(a._x - b._x, a._y - b._y);
}

Vector2 operator*(float scalar, const Vector2& other)
{
    return Vector2(other._x * scalar, other._y * scalar);
}

Vector2 operator*(const Vector2& other, float scalar)
{
    return Vector2(other._x * scalar, other._y * scalar);
}

Vector2 operator*(const Vector2& a, const Vector2& b)
{
    return Vector2(a._x * b._x, a._y * b._y);
}

bool operator==(const Vector2& a, const Vector2& b)
{
    return a._x == b._x && a._y == b._y;
}

bool operator!=(const Vector2& a, const Vector2& b)
{
    return !(a == b);
}

Vector2 lerp(Vector2& left, Vector2& right, float t)
{
    return Vector2(left + t * (right - left));
}

Vector2::Vector2(float x, float y) :
_x(x),
_y(y)
{
    // Empty
}

Vector2::Vector2(const Vector2& other)
{
    _x = other._x;
    _y = other._y;
}

Vector2& Vector2::set(const Vector2& other)
{
    _x = other._x;
    _y = other._y;
    
    return *this;
}

Vector2& Vector2::set(float x, float y)
{
    _x = x;
    _y = y;
    
    return *this;
}

Vector2& Vector2::setX(float x)
{
    _x = x;
    
    return *this;
}

Vector2& Vector2::setY(float y)
{
    _y = y;
    
    return *this;
}

void Vector2::operator+=(const Vector2& other)
{
    add(other._x, other._y);
}

Vector2& Vector2::add(float x, float y)
{
    _x += x;
    _y += y;
    
    return *this;
}

void Vector2::operator-=(const Vector2& other)
{
    sub(other._x, other._y);
}

Vector2& Vector2::sub(float x, float y)
{
    _x -= x;
    _y -= y;
    
    return *this;
}

Vector2 Vector2::operator*=(float scalar)
{
    return mul(scalar);
}

Vector2& Vector2::mul(float scalar)
{
    _x *= scalar;
    _y *= scalar;
    
    return *this;
}

Vector2 Vector2::operator/=(float scalar)
{
    return div(scalar);
}

Vector2& Vector2::div(float scalar)
{
    _x /= scalar;
    _y /= scalar;
    
    return *this;
}

Vector2 Vector2::cpy()
{
    return Vector2(_x, _y);
}

float Vector2::len()
{
    return sqrtf(_x * _x + _y * _y);
}

float Vector2::lenSquared()
{
    return _x * _x + _y * _y;
}

Vector2& Vector2::nor()
{
    float l = len();
    
    if (l != 0)
    {
        _x /= l;
        _y /= l;
    }
    
    return *this;
}

float Vector2::angle()
{
    float radians = atan2f(_y, _x);
    float angle = static_cast<float>(RADIANS_TO_DEGREES(radians));
    
    if (angle < 0)
    {
        angle += 360;
    }
    
    return angle;
}

Vector2& Vector2::rotate(float angle)
{
    float rad = static_cast<float>(DEGREES_TO_RADIANS(angle));
    float cos = cosf(rad);
    float sin = sinf(rad);
    
    float newX = _x * cos - _y * sin;
    float newY = _x * sin + _y * cos;
    
    _x = newX;
    _y = newY;
    
    return *this;
}

float Vector2::dot(const Vector2& other)
{
    return (_x * other._x + _y * other._y);
}

float Vector2::dist(const Vector2& other)
{
    return dist(other._x, other._y);
}

float Vector2::dist(float x, float y) const
{
    return sqrtf(distSquared(x, y));
}

float Vector2::distSquared(const Vector2& other) const
{
    return distSquared(other._x, other._y);
}

float Vector2::distSquared(float x, float y) const
{
    float distX = _x - x;
    float distY = _y - y;
    
    return distX * distX + distY * distY;
}

void Vector2::reset()
{
    _x = 0;
    _y = 0;
}
