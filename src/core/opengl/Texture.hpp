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
    uint32_t _texture;
    
    Texture(TextureDescriptor desc) :
    _desc(desc),
    _width(0),
    _height(0),
    _numChannels(0),
    _data(NULL),
    _texture(0)
    {
        // Empty
    }
};
