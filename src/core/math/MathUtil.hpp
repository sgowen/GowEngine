//
//  MathUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <float.h>

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
