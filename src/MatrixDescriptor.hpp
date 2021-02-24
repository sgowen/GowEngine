//
//  MatrixDescriptor.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/23/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

struct MatrixDescriptor
{
    float _left;
    float _right;
    float _bottom;
    float _top;
    float _near;
    float _far;
    
    MatrixDescriptor(float left, float right, float bottom, float top, float near = -1, float far = 1) :
    _left(left),
    _right(right),
    _bottom(bottom),
    _top(top),
    _near(near),
    _far(far)
    {
        // Empty
    }
    
    MatrixDescriptor(float width, float height) :
    _left(0),
    _right(width),
    _bottom(0),
    _top(height),
    _near(-1),
    _far(1)
    {
        // Empty
    }
};
