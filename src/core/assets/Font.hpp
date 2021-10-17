//
//  Font.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 10/17/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>

struct Font
{
    const std::string _name;
    const std::string _texture;
    const uint8_t _glyphsPerRow;
    const uint8_t _glyphWidth;
    const uint8_t _glyphHeight;
    const float _glyphWidthToHeightRatio;
    
    Font(std::string name, std::string texture, uint8_t glyphsPerRow, uint8_t glyphWidth, uint8_t glyphHeight) :
    _name(name),
    _texture(texture),
    _glyphsPerRow(glyphsPerRow),
    _glyphWidth(glyphWidth),
    _glyphHeight(glyphHeight),
    _glyphWidthToHeightRatio(_glyphHeight / (float)_glyphWidth)
    {
        // Empty
    }
};
