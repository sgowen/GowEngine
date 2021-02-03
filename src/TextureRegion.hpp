//
//  TextureRegion.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

struct TextureRegion
{
    const int _x, _y, _width, _height, _textureWidth, _textureHeight;
    const double _u1, _v1, _u2, _v2;
    
    TextureRegion(int x, int y, int regionWidth, int regionHeight, int textureWidth, int textureHeight) :
    _x(x),
    _y(y),
    _width(regionWidth),
    _height(regionHeight),
    _textureWidth(textureWidth),
    _textureHeight(textureHeight),
    _u1(x / (double) textureWidth),
    _v1(y / (double) textureHeight),
    _u2(_u1 + regionWidth / (double) textureWidth),
    _v2(_v1 + regionHeight / (double) textureHeight)
    {
        // Empty
    }
};
