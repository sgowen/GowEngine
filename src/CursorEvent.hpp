//
//  CursorEvent.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

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
    float _x;
    float _y;
    bool _isAlt;
    
    CursorEvent(CursorEventType type = CursorEventType_DOWN, float x = 0, float y = 0, bool isAlt = false) :
    _type(type),
    _x(x),
    _y(y),
    _isAlt(isAlt)
    {
        // Empty
    }
    
    bool isPressed()
    {
        return _type == CursorEventType_DOWN || _type == CursorEventType_DRAGGED;
    }
};
