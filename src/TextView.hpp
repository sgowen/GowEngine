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
    TextAlignment_LEFT,
    TextAlignment_CENTER,
    TextAlignment_RIGHT
};

struct TextView
{
    std::string _text;
    float _x;
    float _y;
    float _glyphWidth;
    float _glyphHeight;
    TextAlignment _textAlignment;
    
    TextView(std::string text, float x, float y, float glyphWidth, float glyphHeight, TextAlignment textAlignment = TextAlignment_LEFT) :
    _text(text),
    _x(x),
    _y(y),
    _glyphWidth(glyphWidth),
    _glyphHeight(glyphHeight),
    _textAlignment(textAlignment)
    {
        // Empty
    }
};
