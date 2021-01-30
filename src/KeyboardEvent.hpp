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
    KBET_DOWN,
    KBET_HELD,
    KBET_UP
};

struct KeyboardEvent
{
    KeyboardEventType _type;
    uint16_t _key;
    
    KeyboardEvent(KeyboardEventType type = KBET_DOWN, uint16_t key = 0) :
    _type(type),
    _key(key)
    {
        // Empty
    }
    
    bool isPressed()
    {
        return _type == KBET_DOWN || _type == KBET_HELD;
    }

    bool isUp()
    {
        return _type == KBET_UP;
    }
};
