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

InputManager& InputManager::getInstance()
{
    static InputManager ret = InputManager();
    return ret;
}

void InputManager::onCursorInput(CursorEventType type, float x, float y, bool isAlt)
{
#if IS_DEBUG
    LOG("onCursorInput %d%", type);
#endif
    
    CursorEvent* e = _poolCursor.newObject();
    e->_type = type;
    e->_x = x;
    e->_y = y;
    e->_isAlt = isAlt;
    
    _poolCursor.add(e);
}

void InputManager::onGamepadInput(GamepadEventType type, uint8_t index, float x, float y)
{
#if IS_DEBUG
    LOG("onGamepadInput %d%", type);
#endif
    
    if (index >= 4)
    {
        // Must be 0, 1, 2, 3 (supports 4 players)
        return;
    }
    
    GamepadEvent* e = _poolGamepad.newObject();
    e->_type = type;
    e->_index = index;
    e->_x = x;
    e->_y = y;
    
    _poolGamepad.add(e);
}

void InputManager::onKeyboardInput(uint16_t key, bool isUp)
{
#if IS_DEBUG
    LOG("onKeyboardInput %d%", key);
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
    e->_type = isUp ? KeyboardEventType_UP : wasLastEventDown ? KeyboardEventType_HELD : KeyboardEventType_DOWN;
    
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

InputManager::InputManager() :
_poolCursor(4096),
_poolGamepad(16384),
_poolKeyboard(4096)
{
    // Empty
}

InputManager::~InputManager()
{
    // Empty
}
