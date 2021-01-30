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
    TEXA_LEFT,
    TEXA_CENTER,
    TEXA_RIGHT
};

enum TextVisibility
{
    TEXV_VISIBLE,
    TEXV_HIDDEN
};

struct TextView
{
    TextAlignment _alignment;
    std::string _text;
    TextVisibility _visibility;
    bool _hidden;
    float _x;
    float _y;
    float _glyphWidth;
    float _glyphHeight;
    
    TextView(TextAlignment alignment = TEXA_LEFT, std::string text = "", TextVisibility visibility = TEXV_VISIBLE, float x = 0, float y = 0, float glyphWidth = 1, float glyphHeight = 1) :
    _alignment(alignment),
    _text(text),
    _visibility(visibility),
    _x(x),
    _y(y),
    _glyphWidth(glyphWidth),
    _glyphHeight(glyphHeight)
    {
        // Empty
    }
};
