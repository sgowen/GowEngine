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
    CursorEventType_DOWN,
    CursorEventType_DRAGGED,
    CursorEventType_MOVED,
    CursorEventType_UP,
    CursorEventType_SCROLL
};

struct CursorEvent
{
    CursorEventType _type;
    Vector2 _pos;
    bool _isAlt;
    
    CursorEvent(CursorEventType type = CursorEventType_DOWN, float x = 0, float y = 0, bool isAlt = false) :
    _type(type),
    _pos(x, y),
    _isAlt(isAlt)
    {
        // Empty
    }
    
    bool isPressed()
    {
        return _type == CursorEventType_DOWN || _type == CursorEventType_DRAGGED;
    }
};
