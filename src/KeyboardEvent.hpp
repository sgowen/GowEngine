//
//  KeyboardEvent.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

enum KeyboardEventType
{
    KeyboardEventType_DOWN,
    KeyboardEventType_HELD,
    KeyboardEventType_UP
};

struct KeyboardEvent
{
    KeyboardEventType _type;
    uint16_t _key;
    
    KeyboardEvent(KeyboardEventType type = KeyboardEventType_DOWN, uint16_t key = 0) :
    _type(type),
    _key(key)
    {
        // Empty
    }
    
    bool isPressed()
    {
        return _type == KeyboardEventType_DOWN || _type == KeyboardEventType_HELD;
    }
    
    bool isDown()
    {
        return _type == KeyboardEventType_DOWN;
    }

    bool isHeld()
    {
        return _type == KeyboardEventType_HELD;
    }

    bool isUp()
    {
        return _type == KeyboardEventType_UP;
    }
};
