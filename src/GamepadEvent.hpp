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
    GamepadEventType_BUTTON_X = 0,
    GamepadEventType_BUTTON_A = 1,
    GamepadEventType_BUTTON_B = 2,
    GamepadEventType_BUTTON_Y = 3,
    GamepadEventType_BUMPER_LEFT = 4,
    GamepadEventType_BUMPER_RIGHT = 5,
    GamepadEventType_UNKNOWN_6 = 6,
    GamepadEventType_UNKNOWN_7 = 7,
    GamepadEventType_BUTTON_SELECT = 8,
    GamepadEventType_BUTTON_START = 9,
    GamepadEventType_UNKNOWN_10 = 10,
    GamepadEventType_UNKNOWN_11 = 11,
    GamepadEventType_UNKNOWN_12 = 12,
    GamepadEventType_UNKNOWN_13 = 13,
    GamepadEventType_D_PAD_UP = 14,
    GamepadEventType_D_PAD_RIGHT = 15,
    GamepadEventType_D_PAD_DOWN = 16,
    GamepadEventType_D_PAD_LEFT = 17,
    GamepadEventType_STICK_LEFT,
    GamepadEventType_STICK_RIGHT,
    GamepadEventType_TRIGGER
};

struct GamepadEvent
{
    GamepadEventType _type;
    uint8_t _index;
    float _x;
    float _y;
    
    GamepadEvent(GamepadEventType type = GamepadEventType_D_PAD_RIGHT, uint8_t index = 0, float x = 0, float y = 0) :
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
