//
//  InputManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "CursorEvent.hpp"
#include "GamepadEvent.hpp"
#include "KeyboardEvent.hpp"
#include "FixedSizePool.hpp"

#include <map>
#include <vector>

#define INPUT_MGR InputManager::getInstance()

class InputManager
{
public:
    static InputManager& getInstance();
    
    void onCursorInput(CursorEventType type, float x, float y, bool isAlt = false);
    void onGamepadInput(GamepadEventType type, uint8_t index, float x = 0, float y = 0);
    void onKeyboardInput(uint16_t key, bool isUp = false);
    void process();
    std::vector<CursorEvent*>& getCursorEvents();
    std::vector<GamepadEvent*>& getGamepadEvents();
    std::vector<KeyboardEvent*>& getKeyboardEvents();
    
private:
    FixedSizePool<CursorEvent> _poolCursor;
    FixedSizePool<GamepadEvent> _poolGamepad;
    FixedSizePool<KeyboardEvent> _poolKeyboard;
    std::map<unsigned short, bool> _lastKnownKeyStates;
    
    InputManager();
    ~InputManager();
    InputManager(const InputManager&);
    InputManager& operator=(const InputManager&);
};
