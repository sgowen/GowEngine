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
    
    MatrixDescriptor(uint32_t width, uint32_t height) :
    _left(0),
    _right(static_cast<float>(width)),
    _bottom(0),
    _top(static_cast<float>(height)),
    _near(-1),
    _far(1)
    {
        // Empty
    }
    
    float centerX()
    {
        return _left + width() / 2;
    }
    
    float centerY()
    {
        return _bottom + height() / 2;
    }
    
    float width()
    {
        return _right - _left;
    }
    
    float height()
    {
        return _top - _bottom;
    }
};
