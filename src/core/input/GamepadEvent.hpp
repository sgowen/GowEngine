//
//  GamepadEvent.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/21/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

enum GamepadEventButtons
{
    GPEB_BUTTON_A = 0,
    GPEB_BUTTON_B = 1,
    GPEB_BUTTON_Y = 2,
    GPEB_BUTTON_X = 3,
    GPEB_BUMPER_LEFT = 4,
    GPEB_BUMPER_RIGHT = 5,
    GPEB_UNKNOWN_6 = 6,
    GPEB_UNKNOWN_7 = 7,
    GPEB_BUTTON_SELECT = 8,
    GPEB_BUTTON_START = 9,
    GPEB_BUTTON_SNES_SELECT = 10,
    GPEB_BUTTON_SNES_START = 11,
    GPEB_UNKNOWN_12 = 12,
    GPEB_UNKNOWN_13 = 13,
    GPEB_D_PAD_UP = 14,
    GPEB_D_PAD_RIGHT = 15,
    GPEB_D_PAD_DOWN = 16,
    GPEB_D_PAD_LEFT = 17,
    GPEB_STICK_LEFT = 100,
    GPEB_STICK_RIGHT = 101,
    GPEB_TRIGGER = 102
};

enum GamepadEventType
{
    GPET_DOWN,
    GPET_HELD,
    GPET_UP
};

struct GamepadEvent
{
    GamepadEventType _type;
    uint8_t _button;
    uint8_t _index;
    float _x;
    float _y;
    
    GamepadEvent(GamepadEventType type = GPET_DOWN, uint8_t button = GPEB_BUTTON_X, uint8_t index = 0, float x = 0, float y = 0) :
    _type(type),
    _button(button),
    _index(index),
    _x(x),
    _y(y)
    {
        // Empty
    }
    
    bool isPressed()
    {
        return _type == GPET_DOWN || _type == GPET_HELD;
    }
    
    bool isDown()
    {
        return _type == GPET_DOWN;
    }
    
    bool isHeld()
    {
        return _type == GPET_HELD;
    }

    bool isUp()
    {
        return _type == GPET_UP;
    }
};
