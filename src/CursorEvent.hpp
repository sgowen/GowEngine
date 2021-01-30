//
//  CursorEvent.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Vector2.hpp"

enum CursorEventType
{
    CUET_DOWN,
    CUET_DRAGGED,
    CUET_MOVED,
    CUET_UP,
    CUET_SCROLL
};

struct CursorEvent
{
    CursorEventType _type;
    Vector2 _pos;
    bool _isAlt;
    
    CursorEvent(CursorEventType type = CUET_DOWN, float x = 0, float y = 0, bool isAlt = false) :
    _type(type),
    _pos(x, y),
    _isAlt(isAlt)
    {
        // Empty
    }
    
    bool isPressed()
    {
        return _type == CUET_DOWN || _type == CUET_DRAGGED;
    }
};
