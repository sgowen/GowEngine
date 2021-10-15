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

#define DEGREES_TO_RADIANS(angle) ( (angle) / 180.0f * M_PI )
#define RADIANS_TO_DEGREES(angle) ( (angle) / M_PI * 180.0f )

inline float sanitizeCloseToZeroValue(float x, float distanceFromZero)
{
    if (x < distanceFromZero && x > -distanceFromZero)
    {
        return 0.0f;
    }
    
    return x;
}

inline void sanitizeCloseToZeroVector(float& x, float& y, float distanceFromZero)
{
    if (x < distanceFromZero && x > -distanceFromZero)
    {
        x = 0.0f;
    }
    
    if (y < distanceFromZero && y > -distanceFromZero)
    {
        y = 0.0f;
    }
}

inline bool areFloatsPracticallyEqual(float A, float B, float maxDiff = 0.000001f, float maxRelDiff = FLT_EPSILON)
{
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
