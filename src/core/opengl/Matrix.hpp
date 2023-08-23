//
//  Matrix.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/23/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/assets/MatrixDescriptor.hpp"

#include "ShaderInput.hpp"

struct Matrix
{
    MatrixDescriptor _base;
    MatrixDescriptor _desc;
    mat4 _matrix;
    
    Matrix(MatrixDescriptor desc) :
    _base(desc),
    _desc(desc)
    {
        ortho(desc._left, desc._right, desc._bottom, desc._top, desc._near, desc._far);
    }
    
    void identity()
    {
        mat4& m = _matrix;
        
        int i, j;
        for (i = 0; i < 4; ++i)
        {
            for (j = 0; j < 4; ++j)
            {
                m[i][j] = i == j ? 1.f : 0.f;
            }
        }
    }
    
    void ortho(float l, float r, float b, float t, float n, float f)
    {
        identity();
        
        mat4& m = _matrix;
        
        m[0][0] = 2.f / (r - l);
        m[0][1] = m[0][2] = m[0][3] = 0.f;
        
        m[1][1] = 2.f / (t - b);
        m[1][0] = m[1][2] = m[1][3] = 0.f;
        
        m[2][2] = -2.f / (f - n);
        m[2][0] = m[2][1] = m[2][3] = 0.f;
        
        m[3][0] = -(r + l) / (r - l);
        m[3][1] = -(t + b) / (t - b);
        m[3][2] = -(f + n) / (f - n);
        m[3][3] = 1.f;
        
        _desc._left = l;
        _desc._right = r;
        _desc._bottom = b;
        _desc._top = t;
        _desc._near = n;
        _desc._far = f;
    }
};
