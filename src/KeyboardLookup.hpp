//
//  KeyboardLookup.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <algorithm>

#define GOW_KEY_SPACE_BAR 0x0020
#define GOW_KEY_ASCII_COMMA 0x002C
#define GOW_KEY_ASCII_PERIOD 0x002E
#define GOW_KEY_ZERO 0x0030
#define GOW_KEY_ONE 0x0031
#define GOW_KEY_TWO 0x0032
#define GOW_KEY_THREE 0x0033
#define GOW_KEY_FOUR 0x0034
#define GOW_KEY_FIVE 0x0035
#define GOW_KEY_SIX 0x0036
#define GOW_KEY_SEVEN 0x0037
#define GOW_KEY_EIGHT 0x0038
#define GOW_KEY_NINE 0x0039
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

inline std::vector<uint16_t>& getSupportedKeys()
{
    static std::vector<uint16_t> keys;
	if (keys.size() == 0)
	{
		keys.push_back(GOW_KEY_BACK_SPACE);
		keys.push_back(GOW_KEY_CARRIAGE_RETURN);
		keys.push_back(GOW_KEY_ESCAPE);
		keys.push_back(GOW_KEY_SPACE_BAR);
		keys.push_back(GOW_KEY_ASCII_PERIOD);
		keys.push_back(GOW_KEY_ASCII_COMMA);
        keys.push_back(GOW_KEY_PERIOD);
		keys.push_back(GOW_KEY_COMMA);
		keys.push_back(GOW_KEY_ZERO);
		keys.push_back(GOW_KEY_ONE);
		keys.push_back(GOW_KEY_TWO);
		keys.push_back(GOW_KEY_THREE);
		keys.push_back(GOW_KEY_FOUR);
		keys.push_back(GOW_KEY_FIVE);
		keys.push_back(GOW_KEY_SIX);
		keys.push_back(GOW_KEY_SEVEN);
		keys.push_back(GOW_KEY_EIGHT);
		keys.push_back(GOW_KEY_NINE);
		keys.push_back(GOW_KEY_COLON);
		keys.push_back(GOW_KEY_A);
		keys.push_back(GOW_KEY_B);
		keys.push_back(GOW_KEY_C);
		keys.push_back(GOW_KEY_D);
		keys.push_back(GOW_KEY_E);
		keys.push_back(GOW_KEY_F);
		keys.push_back(GOW_KEY_G);
		keys.push_back(GOW_KEY_H);
		keys.push_back(GOW_KEY_I);
		keys.push_back(GOW_KEY_J);
		keys.push_back(GOW_KEY_K);
		keys.push_back(GOW_KEY_L);
		keys.push_back(GOW_KEY_M);
		keys.push_back(GOW_KEY_N);
		keys.push_back(GOW_KEY_O);
		keys.push_back(GOW_KEY_P);
		keys.push_back(GOW_KEY_Q);
		keys.push_back(GOW_KEY_R);
		keys.push_back(GOW_KEY_S);
		keys.push_back(GOW_KEY_T);
		keys.push_back(GOW_KEY_U);
		keys.push_back(GOW_KEY_V);
		keys.push_back(GOW_KEY_W);
		keys.push_back(GOW_KEY_X);
		keys.push_back(GOW_KEY_Y);
		keys.push_back(GOW_KEY_Z);
        keys.push_back(GOW_KEY_CTRL);
        keys.push_back(GOW_KEY_CMD);
		keys.push_back(GOW_KEY_DELETE);
		keys.push_back(GOW_KEY_ARROW_UP);
		keys.push_back(GOW_KEY_ARROW_DOWN);
		keys.push_back(GOW_KEY_ARROW_LEFT);
		keys.push_back(GOW_KEY_ARROW_RIGHT);
	}

	return keys;
}

inline std::vector<uint16_t>& getNumericalKeys()
{
    static std::vector<uint16_t> keys;
    if (keys.size() == 0)
    {
        keys.push_back(GOW_KEY_ZERO);
        keys.push_back(GOW_KEY_ONE);
        keys.push_back(GOW_KEY_TWO);
        keys.push_back(GOW_KEY_THREE);
        keys.push_back(GOW_KEY_FOUR);
        keys.push_back(GOW_KEY_FIVE);
        keys.push_back(GOW_KEY_SIX);
        keys.push_back(GOW_KEY_SEVEN);
        keys.push_back(GOW_KEY_EIGHT);
        keys.push_back(GOW_KEY_NINE);
    }
    
    return keys;
}

inline bool isKeySupported(uint16_t key)
{
	std::vector<uint16_t>& keys = getSupportedKeys();

	return std::find(keys.begin(), keys.end(), key) != keys.end();
}

inline bool isKeyNumerical(uint16_t key)
{
    std::vector<uint16_t>& keys = getNumericalKeys();
    
    return std::find(keys.begin(), keys.end(), key) != keys.end();
}
