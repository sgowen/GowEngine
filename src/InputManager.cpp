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
#include "MathUtil.hpp"

#include <algorithm>

void InputManager::onCursorInput(CursorEventType type, float x, float y, bool isAlt)
{
    if (_isLoggingEnabled)
    {
        LOG("onCursorInput %d %f %f %d", type, x, y, isAlt);
    }
    
    CursorEvent* e = _poolCursor.newObject();
    e->_type = type;
    e->_pos.set(x, y);
    e->_isAlt = isAlt;
    
    _poolCursor.add(e);
}

void InputManager::onGamepadInput(uint8_t button, uint8_t index, float x, float y)
{
    if (_isLoggingEnabled)
    {
        LOG("onGamepadInput %d %d %f %f", button, index, x, y);
    }
    
    if (index >= NUM_SUPPORTED_GAMEPADS)
    {
        if (_isLoggingEnabled)
        {
            LOG("Only %d gamepads supported!", NUM_SUPPORTED_GAMEPADS);
        }
        return;
    }
    
    bool wasLastEventDown = false;
    
    auto q = _lastKnownGamepadButtonStates[index].find(button);
    
    if (q != _lastKnownGamepadButtonStates[index].end())
    {
        bool wasUp = q->second;
        wasLastEventDown = !wasUp;
    }
    
    float sanitizedX = sanitizeCloseToZeroValue(x, 0.25f);
    float sanitizedY = sanitizeCloseToZeroValue(y, 0.25f);
    bool isUp = fabsf(sanitizedX) == 0;
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
    if (_isLoggingEnabled)
    {
        LOG("onKeyboardInput %d %d", key, isUp);
    }
    
    if (!isKeySupported(key))
    {
        if (_isLoggingEnabled)
        {
            LOG("Key not supported %d", key);
        }
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
    e->_isChar = isKeySuitableForTextInput(key);
    
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
    _lastConvertedCursorPos.set((v._x / _cursorWidth) * _matrixWidth, (1 - v._y / _cursorHeight) * _matrixHeight);
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

void InputManager::setLoggingEnabled(bool loggingEnabled)
{
    _isLoggingEnabled = loggingEnabled;
}

bool InputManager::isKeyNumerical(uint16_t key)
{
    std::vector<uint16_t>& keys = _numericalKeys;
    
    return std::find(keys.begin(), keys.end(), key) != keys.end();
}

bool InputManager::isKeySuitableForTextInput(uint16_t key)
{
    std::vector<uint16_t>& keys = _textKeys;
    
    return std::find(keys.begin(), keys.end(), key) != keys.end();
}

bool InputManager::isKeySupported(uint16_t key)
{
    std::vector<uint16_t>& keys = _supportedKeys;

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
_matrixHeight(1),
_isLoggingEnabled(false)
{
    _numericalKeys.push_back(GOW_KEY_0);
    _numericalKeys.push_back(GOW_KEY_1);
    _numericalKeys.push_back(GOW_KEY_2);
    _numericalKeys.push_back(GOW_KEY_3);
    _numericalKeys.push_back(GOW_KEY_4);
    _numericalKeys.push_back(GOW_KEY_5);
    _numericalKeys.push_back(GOW_KEY_6);
    _numericalKeys.push_back(GOW_KEY_7);
    _numericalKeys.push_back(GOW_KEY_8);
    _numericalKeys.push_back(GOW_KEY_9);
    
    _textKeys.insert(std::end(_textKeys), std::begin(_numericalKeys), std::end(_numericalKeys));
    _textKeys.push_back(GOW_KEY_A);
    _textKeys.push_back(GOW_KEY_B);
    _textKeys.push_back(GOW_KEY_C);
    _textKeys.push_back(GOW_KEY_D);
    _textKeys.push_back(GOW_KEY_E);
    _textKeys.push_back(GOW_KEY_F);
    _textKeys.push_back(GOW_KEY_G);
    _textKeys.push_back(GOW_KEY_H);
    _textKeys.push_back(GOW_KEY_I);
    _textKeys.push_back(GOW_KEY_J);
    _textKeys.push_back(GOW_KEY_K);
    _textKeys.push_back(GOW_KEY_L);
    _textKeys.push_back(GOW_KEY_M);
    _textKeys.push_back(GOW_KEY_N);
    _textKeys.push_back(GOW_KEY_O);
    _textKeys.push_back(GOW_KEY_P);
    _textKeys.push_back(GOW_KEY_Q);
    _textKeys.push_back(GOW_KEY_R);
    _textKeys.push_back(GOW_KEY_S);
    _textKeys.push_back(GOW_KEY_T);
    _textKeys.push_back(GOW_KEY_U);
    _textKeys.push_back(GOW_KEY_V);
    _textKeys.push_back(GOW_KEY_W);
    _textKeys.push_back(GOW_KEY_X);
    _textKeys.push_back(GOW_KEY_Y);
    _textKeys.push_back(GOW_KEY_Z);
    _textKeys.push_back(GOW_KEY_SPACE_BAR);
    _textKeys.push_back(GOW_KEY_PERIOD);
    _textKeys.push_back(GOW_KEY_COMMA);
    
    _supportedKeys.insert(std::end(_supportedKeys), std::begin(_textKeys), std::end(_textKeys));
    _supportedKeys.push_back(GOW_KEY_BACK_SPACE);
    _supportedKeys.push_back(GOW_KEY_CARRIAGE_RETURN);
    _supportedKeys.push_back(GOW_KEY_ESCAPE);
    _supportedKeys.push_back(GOW_KEY_CTRL);
    _supportedKeys.push_back(GOW_KEY_CMD);
    _supportedKeys.push_back(GOW_KEY_DELETE);
    _supportedKeys.push_back(GOW_KEY_ARROW_UP);
    _supportedKeys.push_back(GOW_KEY_ARROW_DOWN);
    _supportedKeys.push_back(GOW_KEY_ARROW_LEFT);
    _supportedKeys.push_back(GOW_KEY_ARROW_RIGHT);
}
