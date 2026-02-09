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
    GEB_BUTTON_A = 0,
    GEB_BUTTON_B = 1,
    GEB_BUTTON_X = 2,
    GEB_BUTTON_Y = 3,
    GEB_BUMPER_LEFT = 4,
    GEB_BUMPER_RIGHT = 5,
    GEB_BUTTON_BACK = 6,
    GEB_BUTTON_START = 7,
    GEB_BUTTON_GUIDE = 8,
    GEB_D_PAD_UP = 11,
    GEB_D_PAD_RIGHT = 12,
    GEB_D_PAD_DOWN = 13,
    GEB_D_PAD_LEFT = 14,
    GEB_STICK_LEFT = 100,
    GEB_STICK_RIGHT = 101,
    GEB_TRIGGER = 102
};

enum GamepadEventType
{
    GET_DOWN,
    GET_HELD,
    GET_UP
};

struct GamepadEvent
{
    GamepadEventType _type;
    uint8_t _button;
    uint8_t _index;
    float _x;
    float _y;
    
    GamepadEvent(GamepadEventType type = GET_DOWN, uint8_t button = GEB_BUTTON_X, uint8_t index = 0, float x = 0, float y = 0) :
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
        return _type == GET_DOWN || _type == GET_HELD;
    }
    
    bool isDown()
    {
        return _type == GET_DOWN;
    }
    
    bool isHeld()
    {
        return _type == GET_HELD;
    }

    bool isUp()
    {
        return _type == GET_UP;
    }
};
