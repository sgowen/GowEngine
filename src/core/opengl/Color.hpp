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
    static Color CLEAR;
    static Color WHITE;
    static Color BLACK;
    static Color RED;
    static Color GREEN;
    static Color BLUE;
    static Color HALF;
    static Color DIM;
    static Color DOUBLE;
    static Color ONE;
    
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
