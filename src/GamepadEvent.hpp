//
//  GamepadEvent.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/21/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

enum GamepadEventType
{
    GPET_BUTTON_X = 0,
    GPET_BUTTON_A = 1,
    GPET_BUTTON_B = 2,
    GPET_BUTTON_Y = 3,
    GPET_BUMPER_LEFT = 4,
    GPET_BUMPER_RIGHT = 5,
    GPET_UNKNOWN_6 = 6,
    GPET_UNKNOWN_7 = 7,
    GPET_BUTTON_SELECT = 8,
    GPET_BUTTON_START = 9,
    GPET_UNKNOWN_10 = 10,
    GPET_UNKNOWN_11 = 11,
    GPET_UNKNOWN_12 = 12,
    GPET_UNKNOWN_13 = 13,
    GPET_D_PAD_UP = 14,
    GPET_D_PAD_RIGHT = 15,
    GPET_D_PAD_DOWN = 16,
    GPET_D_PAD_LEFT = 17,
    GPET_STICK_LEFT,
    GPET_STICK_RIGHT,
    GPET_TRIGGER
};

struct GamepadEvent
{
    GamepadEventType _type;
    uint8_t _index;
    float _x;
    float _y;
    
    GamepadEvent(GamepadEventType type = GPET_D_PAD_RIGHT, uint8_t index = 0, float x = 0, float y = 0) :
    _type(type),
    _index(index),
    _x(x),
    _y(y)
    {
        // Empty
    }
    
    bool isPressed()
    {
        return _x > 0;
    }
};
