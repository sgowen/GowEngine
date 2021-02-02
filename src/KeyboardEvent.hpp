//
//  KeyboardEvent.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#define GOW_KEY_SPACE_BAR 0x0020
#define GOW_KEY_ASCII_COMMA 0x002C
#define GOW_KEY_ASCII_PERIOD 0x002E
#define GOW_KEY_ZERO 0x0030
#define GOW_KEY_1 0x0031
#define GOW_KEY_2 0x0032
#define GOW_KEY_3 0x0033
#define GOW_KEY_4 0x0034
#define GOW_KEY_5 0x0035
#define GOW_KEY_6 0x0036
#define GOW_KEY_7 0x0037
#define GOW_KEY_8 0x0038
#define GOW_KEY_9 0x0039
#define GOW_KEY_COLON 0x003A
#define GOW_KEY_A 0x0041
#define GOW_KEY_B 0x0042
#define GOW_KEY_C 0x0043
#define GOW_KEY_D 0x0044
#define GOW_KEY_E 0x0045
#define GOW_KEY_F 0x0046
#define GOW_KEY_G 0x0047
#define GOW_KEY_H 0x0048
#define GOW_KEY_I 0x0049
#define GOW_KEY_J 0x004A
#define GOW_KEY_K 0x004B
#define GOW_KEY_L 0x004C
#define GOW_KEY_M 0x004D
#define GOW_KEY_N 0x004E
#define GOW_KEY_O 0x004F
#define GOW_KEY_P 0x0050
#define GOW_KEY_Q 0x0051
#define GOW_KEY_R 0x0052
#define GOW_KEY_S 0x0053
#define GOW_KEY_T 0x0054
#define GOW_KEY_U 0x0055
#define GOW_KEY_V 0x0056
#define GOW_KEY_W 0x0057
#define GOW_KEY_X 0x0058
#define GOW_KEY_Y 0x0059
#define GOW_KEY_Z 0x005A
#define GOW_KEY_CTRL 341
#define GOW_KEY_CMD 343
#define GOW_KEY_DELETE 0x007F
#define GOW_KEY_BACK_SPACE 0x0103
#define GOW_KEY_ESCAPE 0x0100
#define GOW_KEY_PERIOD GOW_KEY_ASCII_PERIOD
#define GOW_KEY_COMMA GOW_KEY_ASCII_COMMA
#define GOW_KEY_ARROW_LEFT 0x0107
#define GOW_KEY_ARROW_UP 0x0109
#define GOW_KEY_ARROW_RIGHT 0x0106
#define GOW_KEY_ARROW_DOWN 0x0108
#define GOW_KEY_CARRIAGE_RETURN 0x0101

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
    bool _isNumerical;
    
    KeyboardEvent(KeyboardEventType type = KBET_DOWN, uint16_t key = 0, bool isNumerical = false) :
    _type(type),
    _key(key),
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
