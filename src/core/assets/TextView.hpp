//
//  TextView.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/28/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>

enum TextAlignment
{
    TEXA_LEFT = 0,
    TEXA_CENTER,
    TEXA_RIGHT
};

enum TextVisibility
{
    TEXV_HIDDEN = 0,
    TEXV_VISIBLE
};

struct TextView
{
    std::string _text;
    uint8_t _alignment;
    float _xWeight;
    float _yWeight;
    float _glyphWidthWeight;
    uint8_t _visibility;
    
    TextView(std::string text, uint8_t alignment, float xWeight, float yWeight, float glyphWidthWeight) :
    _text(text),
    _alignment(alignment),
    _xWeight(xWeight),
    _yWeight(yWeight),
    _glyphWidthWeight(glyphWidthWeight),
    _visibility(TEXV_VISIBLE)
    {
        // Empty
    }
};
