//
//  ShaderInput.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

typedef int ivec4[4];
typedef float vec2[2];
typedef float vec4[4];
typedef vec4 mat4[4];

struct VERTEX_2D_TEXTURE
{
    vec2 _xy;
    vec2 _uv;
    
    VERTEX_2D_TEXTURE(float x, float y, float u, float v)
    {
        _xy[0] = x;
        _xy[1] = y;
        _uv[0] = u;
        _uv[1] = v;
    }
    
    VERTEX_2D_TEXTURE()
    {
        _xy[0] = 0;
        _xy[1] = 0;
        _uv[0] = 0;
        _uv[1] = 0;
    }
};

struct VERTEX_2D
{
    vec2 _xy;
    
    VERTEX_2D(float x, float y)
    {
        _xy[0] = x;
        _xy[1] = y;
    }
    
    VERTEX_2D()
    {
        _xy[0] = 0;
        _xy[1] = 0;
    }
};

inline void mat4_identity(mat4& m)
{
    int i, j;
    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            m[i][j] = i == j ? 1.f : 0.f;
        }
    }
}

inline void mat4_ortho(mat4& m, float l, float r, float b, float t, float n, float f)
{
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
}
