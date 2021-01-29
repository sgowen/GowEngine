//
//  Framebuffer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

struct Framebuffer
{
    int _width;
    int _height;
    uint32_t _texture;
    uint32_t _fbo;

    Framebuffer(int width, int height) :
    _width(width),
    _height(height),
    _texture(0),
    _fbo(0)
    {
        // Empty
    }
};
