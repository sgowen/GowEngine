//
//  Color.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#define COLOR_CLEAR 0.0f, 0.0f, 0.0f, 0.0f
#define COLOR_WHITE 1.0f, 1.0f, 1.0f, 1.0f
#define COLOR_BLACK 0.0f, 0.0f, 0.0f, 1.0f
#define COLOR_RED 1.0f, 0.0f, 0.0f, 1.0f
#define COLOR_GREEN 0.0f, 1.0f, 0.0f, 1.0f
#define COLOR_BLUE 0.0f, 0.0f, 1.0f, 1.0f
#define COLOR_HALF 0.5f, 0.5f, 0.5f, 0.5f
#define COLOR_DIM 0.1f, 0.1f, 0.1f, 0.9f
#define COLOR_DOUBLE 2.0f, 2.0f, 2.0f, 1.0f
#define COLOR_ONE 1.0f, 1.0f, 1.0f, 1.0f

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
