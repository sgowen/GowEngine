//
//  Texture.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/assets/TextureDescriptor.hpp"

struct Texture
{
    TextureDescriptor _desc;
    int _width;
    int _height;
    int _numChannels;
    uint8_t* _data;
    uint32_t _format;
    uint32_t _texture;
    
    Texture(TextureDescriptor td) :
    _desc(td),
    _width(0),
    _height(0),
    _numChannels(0),
    _data(nullptr),
    _texture(0)
    {
        // Empty
    }
};
