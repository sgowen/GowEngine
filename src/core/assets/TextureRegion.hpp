//
//  TextureRegion.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

struct TextureRegion
{
    const uint16_t _x, _y, _width, _height, _textureWidth, _textureHeight;
    const float _u1, _v1, _u2, _v2;
    
    TextureRegion(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t textureWidth, uint16_t textureHeight) :
    _x(x),
    _y(y),
    _width(width),
    _height(height),
    _textureWidth(textureWidth),
    _textureHeight(textureHeight),
    _u1(x / (float) textureWidth),
    _v1(y / (float) textureHeight),
    _u2(_u1 + width / (float) textureWidth),
    _v2(_v1 + height / (float) textureHeight)
    {
        // Empty
    }
    
    TextureRegion copyWithX(uint16_t x)
    {
        return TextureRegion(x, _y, _width, _height, _textureWidth, _textureHeight);
    }
};
