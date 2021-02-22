//
//  TextView.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/28/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
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
    TextAlignment _alignment;
    float _xWeight;
    float _yWeight;
    float _glyphWidthWeight;
    TextVisibility _visibility;
    
    TextView(std::string text, TextAlignment ta, float xWeight, float yWeight, float glyphWidthWeight) :
    _alignment(ta),
    _text(text),
    _xWeight(xWeight),
    _yWeight(yWeight),
    _glyphWidthWeight(glyphWidthWeight),
    _visibility(TEXV_VISIBLE)
    {
        // Empty
    }
};
