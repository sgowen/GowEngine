//
//  Line.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Vector2.hpp"

struct Line
{
    Vector2 _origin;
    Vector2 _end;
    
    Line(float oX, float oY, float eX, float eY) :
    _origin(oX, oY),
    _end(eX, eY)
    {
        // Empty
    }
};
