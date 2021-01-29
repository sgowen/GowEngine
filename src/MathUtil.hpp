//
//  MathUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <float.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define DEGREES_TO_RADIANS(angle) ( (angle) / 180.0f * M_PI )
#define RADIANS_TO_DEGREES(angle) ( (angle) / M_PI * 180.0f )

inline float clamp(float x, float lower, float upper)
{
    return fminf(upper, fmaxf(x, lower));
}

inline float sanitizeCloseToZeroValue(float x)
{
    if (x < 0.05f && x > -0.05f)
    {
        return 0.0f;
    }
    
    return x;
}

inline bool areFloatsPracticallyEqual(float A, float B, float maxDiff = 0.000001f, float maxRelDiff = FLT_EPSILON)
{
    // Check if the numbers are really close -- needed
    // when comparing numbers near zero.
    float diff = fabs(A - B);
    if (diff <= maxDiff)
    {
        return true;
    }
    
    A = fabs(A);
    B = fabs(B);
    
    float largest = (B > A) ? B : A;
    
    if (diff <= largest * maxRelDiff)
    {
        return true;
    }
    
    return false;
}
