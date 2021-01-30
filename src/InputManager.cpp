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
#include "Network.hpp"
#include "MathUtil.hpp"

InputManager& InputManager::getInstance()
{
    static InputManager ret = InputManager();
    return ret;
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

void InputManager::onGamepadInput(GamepadEventType type, uint8_t index, float x, float y)
{
#if IS_DEBUG
    LOG("onGamepadInput %d", type);
#endif
    
    if (index >= NW_MAX_NUM_PLAYERS)
    {
        // Must be 0, 1, 2, 3 (supports 4 players)
        return;
    }
    
    GamepadEvent* e = _poolGamepad.newObject();
    e->_type = type;
    e->_index = index;
    e->_x = sanitizeCloseToZeroValue(x);
    e->_y = sanitizeCloseToZeroValue(y);
    
    _poolGamepad.add(e);
}

void InputManager::onKeyboardInput(uint16_t key, bool isUp)
{
#if IS_DEBUG
    LOG("onKeyboardInput %d", key);
#endif
    
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
    // Empty
}
