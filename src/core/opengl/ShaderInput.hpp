//
//  ShaderInput.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

typedef int ivec4[4];
typedef float vec2[2];
typedef float vec4[4];
typedef vec4 mat4[4];

struct VERTEX_2D_TEXTURE
{
    vec4 _xyuv;
    
    VERTEX_2D_TEXTURE(float x, float y, float u, float v)
    {
        _xyuv[0] = x;
        _xyuv[1] = y;
        _xyuv[2] = u;
        _xyuv[3] = v;
    }
    
    VERTEX_2D_TEXTURE()
    {
        _xyuv[0] = 0;
        _xyuv[1] = 0;
        _xyuv[2] = 0;
        _xyuv[3] = 0;
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
