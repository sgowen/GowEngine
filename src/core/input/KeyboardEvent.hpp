//
//  KeyboardEvent.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

enum KeyboardEventType
{
    KBET_DOWN,
    KBET_HELD,
    KBET_UP
};

struct KeyboardEvent
{
    KeyboardEventType _type;
    uint16_t _key;
    bool _isChar;
    bool _isNumerical;
    
    KeyboardEvent(KeyboardEventType type = KBET_DOWN, uint16_t key = 0, bool isChar = false, bool isNumerical = false) :
    _type(type),
    _key(key),
    _isChar(isChar),
    _isNumerical(isNumerical)
    {
        // Empty
    }
    
    bool isPressed()
    {
        return _type == KBET_DOWN || _type == KBET_HELD;
    }
    
    bool isDown()
    {
        return _type == KBET_DOWN;
    }
    
    bool isHeld()
    {
        return _type == KBET_HELD;
    }

    bool isUp()
    {
        return _type == KBET_UP;
    }
};
