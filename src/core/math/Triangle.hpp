//
//  Triangle.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Line.hpp"
#include "Vector2.hpp"

struct Triangle
{
    Line _sideA;
    Line _sideB;
    Line _sideC;
    
    Triangle(float x1, float y1, float x2, float y2, float x3, float y3) :
    _sideA(x1, y1, x2, y2),
    _sideB(x2, y2, x3, y3),
    _sideC(x3, y3, x1, y1)
    {
        // Empty
    }
    
    void set(float x1, float y1, float x2, float y2, float x3, float y3)
    {
        _sideA._origin.set(x1, y1);
        _sideA._end.set(x2, y2);
        _sideB._origin.set(x2, y2);
        _sideB._end.set(x3, y3);
        _sideC._origin.set(x3, y3);
        _sideC._end.set(x1, y1);
    }

    Vector2& getPointA()
    {
        return _sideA._origin;
    }

    Vector2& getPointB()
    {
        return _sideB._origin;
    }

    Vector2& getPointC()
    {
        return _sideC._origin;
    }
};
