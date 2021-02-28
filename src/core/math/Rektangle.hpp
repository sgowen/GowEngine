//
//  Rektangle.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Vector2.hpp"

struct Rektangle
{
    Vector2 _lowerLeft;
    float _width;
    float _height;
    float _angle;
    
    Rektangle(float x, float y, float width, float height, float angle = 0) :
    _lowerLeft(x, y),
    _width(width),
    _height(height),
    _angle(angle)
    {
        // Empty
    }
    
    Rektangle() :
    _lowerLeft(0, 0),
    _width(1),
    _height(1),
    _angle(0)
    {
        // Empty
    }
    
    float left()
    {
        return _lowerLeft._x;
    }
    
    float centerX()
    {
        return _lowerLeft._x + _width / 2;
    }

    float right()
    {
        return _lowerLeft._x + _width;
    }

    float bottom()
    {
        return _lowerLeft._y;
    }
    
    float centerY()
    {
        return _lowerLeft._y + _height / 2;
    }

    float top()
    {
        return _lowerLeft._y + _height;
    }
};
