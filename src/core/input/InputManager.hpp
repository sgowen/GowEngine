//
//  InputManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/FixedSizePool.hpp"
#include "core/math/Vector2.hpp"

#include "CursorEvent.hpp"
#include "GamepadEvent.hpp"
#include "KeyboardEvent.hpp"

#include <map>
#include <vector>

class ClipboardHandler;
struct Matrix;

class InputManager
{
public:
    static InputManager& getInstance()
    {
        static InputManager ret = InputManager();
        return ret;
    }
    
    void onCursorInput(CursorEventType type, float x, float y, bool isAlt = false);
    void onGamepadInput(uint8_t button, uint8_t index, float x, float y = 0);
    void onKeyboardInput(uint16_t key, bool isUp);
    void process();
    std::vector<CursorEvent*>& getCursorEvents();
    std::vector<GamepadEvent*>& getGamepadEvents();
    std::vector<KeyboardEvent*>& getKeyboardEvents();
    Vector2& convert(CursorEvent* ce);
    Vector2& convert(Vector2& v);
    void setClipboardHandler(ClipboardHandler* clipboardHandler);
    const char* getClipboardString();
    void setCursorSize(int cursorWidth, int cursorHeight);
    void setMatrix(Matrix* m);
    
    
private:
    FixedSizePool<CursorEvent> _poolCursor;
    FixedSizePool<GamepadEvent> _poolGamepad;
    FixedSizePool<KeyboardEvent> _poolKeyboard;
    std::vector<uint16_t> _numericalKeys;
    std::vector<uint16_t> _textKeys;
    std::vector<uint16_t> _supportedKeys;
    std::map<uint16_t, bool> _lastKnownKeyStates;
    std::map<uint8_t, bool> _lastKnownGamepadButtonStates[MAX_NUM_PLAYERS];
    Vector2 _lastConvertedCursorPos;
    int _cursorWidth;
    int _cursorHeight;
    ClipboardHandler* _clipboardHandler;
    Matrix* _matrix;
    
    bool isKeyNumerical(uint16_t key);
    bool isKeySuitableForTextInput(uint16_t key);
    bool isKeySupported(uint16_t key);
    
    InputManager();
    ~InputManager() {}
    InputManager(const InputManager&);
    InputManager& operator=(const InputManager&);
};
