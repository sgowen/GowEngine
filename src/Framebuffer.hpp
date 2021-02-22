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
    uint16_t _width;
    uint16_t _height;
    const std::string _filterMin;
    const std::string _filterMag;
    uint32_t _texture;
    uint32_t _fbo;

    Framebuffer(uint16_t width, uint16_t height, std::string filterMin, std::string filterMag) :
    _width(width),
    _height(height),
    _filterMin(filterMin),
    _filterMag(filterMag),
    _texture(0),
    _fbo(0)
    {
        // Empty
    }
};
