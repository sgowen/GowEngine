//
//  Circle.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 8/6/13.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Vector2.hpp"

struct Circle
{
    Vector2 _center;
    float _radius;
    
	Circle(float x, float y, float radius) :
    _center(x, y),
    _radius(radius)
    {
        // Empty
    }
};
