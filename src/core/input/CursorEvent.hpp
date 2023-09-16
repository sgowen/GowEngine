//
//  CursorEvent.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/math/Vector2.hpp"

enum CursorEventType
{
    CUET_DOWN,
    CUET_DRAGGED,
    CUET_UP,
    CUET_MOVED,
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
    
    bool isDown()
    {
        return _type == CUET_DOWN;
    }
    
    bool isHeld()
    {
        return _type == CUET_DRAGGED;
    }

    bool isUp()
    {
        return _type == CUET_UP;
    }
};
