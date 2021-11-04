//
//  Color.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

struct Color
{
    static const Color CLEAR;
    static const Color WHITE;
    static const Color BLACK;
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    static const Color HALF;
    static const Color DIM;
    static const Color DOUBLE;
    static const Color ONE;
    
    float _red;
    float _green;
    float _blue;
    float _alpha;

    Color(float red, float green, float blue, float alpha) :
    _red(red),
    _green(green),
    _blue(blue),
    _alpha(alpha)
    {
        // Empty
    }

    Color() :
    _red(0),
    _green(0),
    _blue(0),
    _alpha(0)
    {
        // Empty
    }
};
