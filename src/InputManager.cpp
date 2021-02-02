//
//  InputManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "InputManager.hpp"

#include "StringUtil.hpp"
#include "PlatformMacros.hpp"

#include <algorithm>

inline float sanitizeCloseToZeroValue(float x)
{
    if (x < 0.25f && x > -0.25f)
    {
        return 0.0f;
    }
    
    return x;
}

void InputManager::onCursorInput(CursorEventType type, float x, float y, bool isAlt)
{
#if IS_DEBUG
    LOG("onCursorInput %d", type);
#endif
    
    CursorEvent* e = _poolCursor.newObject();
    e->_type = type;
    e->_pos.set(x, y);
    e->_isAlt = isAlt;
    
    _poolCursor.add(e);
}

void InputManager::onGamepadInput(uint8_t button, uint8_t index, float x, float y)
{
#if IS_DEBUG
    LOG("onGamepadInput %d", button);
#endif
    
    if (index >= NUM_SUPPORTED_GAMEPADS)
    {
#if IS_DEBUG
    LOG("Only %d gamepads supported!", NUM_SUPPORTED_GAMEPADS);
#endif
        return;
    }
    
    bool wasLastEventDown = false;
    
    auto q = _lastKnownGamepadButtonStates[index].find(button);
    
    if (q != _lastKnownGamepadButtonStates[index].end())
    {
        bool wasUp = q->second;
        wasLastEventDown = !wasUp;
    }
    
    float sanitizedX = sanitizeCloseToZeroValue(x);
    float sanitizedY = sanitizeCloseToZeroValue(y);
    bool isUp = sanitizedX <= 0;
    _lastKnownGamepadButtonStates[index][button] = isUp;
    
    GamepadEvent* e = _poolGamepad.newObject();
    e->_type = isUp ? GPET_UP : wasLastEventDown ? GPET_HELD : GPET_DOWN;
    e->_button = button;
    e->_index = index;
    e->_x = sanitizedX;
    e->_y = sanitizedY;
    
    _poolGamepad.add(e);
}

void InputManager::onKeyboardInput(uint16_t key, bool isUp)
{
#if IS_DEBUG
    LOG("onKeyboardInput %d", key);
#endif
    
    if (!isKeySupported(key))
    {
#if IS_DEBUG
    LOG("Key not supported %d", key);
#endif
        return;
    }
    
    bool wasLastEventDown = false;
    
    auto q = _lastKnownKeyStates.find(key);
    
    if (q != _lastKnownKeyStates.end())
    {
        bool wasUp = q->second;
        wasLastEventDown = !wasUp;
    }
    
    _lastKnownKeyStates[key] = isUp;
    
    KeyboardEvent* e = _poolKeyboard.newObject();
    e->_key = key;
    e->_type = isUp ? KBET_UP : wasLastEventDown ? KBET_HELD : KBET_DOWN;
    e->_isNumerical = isKeyNumerical(key);
    
    _poolKeyboard.add(e);
}

void InputManager::process()
{
    _poolCursor.processBuffer();
    _poolGamepad.processBuffer();
    _poolKeyboard.processBuffer();
}

std::vector<CursorEvent*>& InputManager::getCursorEvents()
{
    return _poolCursor.getObjects();
}

std::vector<GamepadEvent*>& InputManager::getGamepadEvents()
{
    return _poolGamepad.getObjects();
}

std::vector<KeyboardEvent*>& InputManager::getKeyboardEvents()
{
    return _poolKeyboard.getObjects();
}

Vector2& InputManager::convert(CursorEvent* ce)
{
    return convert(ce->_pos);
}

Vector2& InputManager::convert(Vector2& v)
{
    _lastConvertedCursorPos.set((v.x() / _cursorWidth) * _matrixWidth, (1 - v.y() / _cursorHeight) * _matrixHeight);
    return _lastConvertedCursorPos;
}

void InputManager::setCursorSize(int cursorWidth, int cursorHeight)
{
    _cursorWidth = (float) cursorWidth;
    _cursorHeight = (float) cursorHeight;
}

void InputManager::setMatrixSize(float matrixWidth, float matrixHeight)
{
    _matrixWidth = matrixWidth;
    _matrixHeight = matrixHeight;
}

bool InputManager::isKeySupported(uint16_t key)
{
    std::vector<uint16_t>& keys = _supportedKeys;

    return std::find(keys.begin(), keys.end(), key) != keys.end();
}

bool InputManager::isKeyNumerical(uint16_t key)
{
    std::vector<uint16_t>& keys = _numericalKeys;
    
    return std::find(keys.begin(), keys.end(), key) != keys.end();
}

InputManager::InputManager() :
_poolCursor(4096),
_poolGamepad(16384),
_poolKeyboard(4096),
_lastConvertedCursorPos(),
_cursorWidth(1),
_cursorHeight(1),
_matrixWidth(1),
_matrixHeight(1)
{
    _supportedKeys.push_back(GOW_KEY_BACK_SPACE);
    _supportedKeys.push_back(GOW_KEY_CARRIAGE_RETURN);
    _supportedKeys.push_back(GOW_KEY_ESCAPE);
    _supportedKeys.push_back(GOW_KEY_SPACE_BAR);
    _supportedKeys.push_back(GOW_KEY_ASCII_PERIOD);
    _supportedKeys.push_back(GOW_KEY_ASCII_COMMA);
    _supportedKeys.push_back(GOW_KEY_PERIOD);
    _supportedKeys.push_back(GOW_KEY_COMMA);
    _supportedKeys.push_back(GOW_KEY_ZERO);
    _supportedKeys.push_back(GOW_KEY_1);
    _supportedKeys.push_back(GOW_KEY_2);
    _supportedKeys.push_back(GOW_KEY_3);
    _supportedKeys.push_back(GOW_KEY_4);
    _supportedKeys.push_back(GOW_KEY_5);
    _supportedKeys.push_back(GOW_KEY_6);
    _supportedKeys.push_back(GOW_KEY_7);
    _supportedKeys.push_back(GOW_KEY_8);
    _supportedKeys.push_back(GOW_KEY_9);
    _supportedKeys.push_back(GOW_KEY_COLON);
    _supportedKeys.push_back(GOW_KEY_A);
    _supportedKeys.push_back(GOW_KEY_B);
    _supportedKeys.push_back(GOW_KEY_C);
    _supportedKeys.push_back(GOW_KEY_D);
    _supportedKeys.push_back(GOW_KEY_E);
    _supportedKeys.push_back(GOW_KEY_F);
    _supportedKeys.push_back(GOW_KEY_G);
    _supportedKeys.push_back(GOW_KEY_H);
    _supportedKeys.push_back(GOW_KEY_I);
    _supportedKeys.push_back(GOW_KEY_J);
    _supportedKeys.push_back(GOW_KEY_K);
    _supportedKeys.push_back(GOW_KEY_L);
    _supportedKeys.push_back(GOW_KEY_M);
    _supportedKeys.push_back(GOW_KEY_N);
    _supportedKeys.push_back(GOW_KEY_O);
    _supportedKeys.push_back(GOW_KEY_P);
    _supportedKeys.push_back(GOW_KEY_Q);
    _supportedKeys.push_back(GOW_KEY_R);
    _supportedKeys.push_back(GOW_KEY_S);
    _supportedKeys.push_back(GOW_KEY_T);
    _supportedKeys.push_back(GOW_KEY_U);
    _supportedKeys.push_back(GOW_KEY_V);
    _supportedKeys.push_back(GOW_KEY_W);
    _supportedKeys.push_back(GOW_KEY_X);
    _supportedKeys.push_back(GOW_KEY_Y);
    _supportedKeys.push_back(GOW_KEY_Z);
    _supportedKeys.push_back(GOW_KEY_CTRL);
    _supportedKeys.push_back(GOW_KEY_CMD);
    _supportedKeys.push_back(GOW_KEY_DELETE);
    _supportedKeys.push_back(GOW_KEY_ARROW_UP);
    _supportedKeys.push_back(GOW_KEY_ARROW_DOWN);
    _supportedKeys.push_back(GOW_KEY_ARROW_LEFT);
    _supportedKeys.push_back(GOW_KEY_ARROW_RIGHT);
    
    _numericalKeys.push_back(GOW_KEY_ZERO);
    _numericalKeys.push_back(GOW_KEY_1);
    _numericalKeys.push_back(GOW_KEY_2);
    _numericalKeys.push_back(GOW_KEY_3);
    _numericalKeys.push_back(GOW_KEY_4);
    _numericalKeys.push_back(GOW_KEY_5);
    _numericalKeys.push_back(GOW_KEY_6);
    _numericalKeys.push_back(GOW_KEY_7);
    _numericalKeys.push_back(GOW_KEY_8);
    _numericalKeys.push_back(GOW_KEY_9);
}
