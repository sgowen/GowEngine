//
//  InputManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <algorithm>

void InputManager::onCursorInput(CursorEventType type, float x, float y, bool isAlt)
{
    CursorEvent* e = _poolCursor.newObject();
    e->_type = type;
    e->_pos.set(x, y);
    e->_isAlt = isAlt;
    
    _poolCursor.add(e);
    
    if (ENGINE_CFG.inputLoggingEnabled())
    {
        LOG("onCursorInput %d %f %f %d", type, x, y, isAlt);
    }
}

void InputManager::onGamepadInput(uint8_t button, uint8_t index, float x, float y)
{
    if (index >= MAX_NUM_PLAYERS)
    {
        if (ENGINE_CFG.inputLoggingEnabled())
        {
            LOG("Only %d gamepads supported!", MAX_NUM_PLAYERS);
        }
        return;
    }
    
    bool wasLastEventUp = false;
    
    auto q = _lastKnownGamepadButtonStates[index].find(button);
    if (q != _lastKnownGamepadButtonStates[index].end())
    {
        bool wasUp = q->second;
        wasLastEventUp = wasUp;
    }
    
    float sanitizedX = sanitizeCloseToZeroValue(x, 0.25f);
    float sanitizedY = sanitizeCloseToZeroValue(y, 0.25f);
    bool isUp = fabsf(sanitizedX) == 0 && fabsf(sanitizedY) == 0;
    _lastKnownGamepadButtonStates[index][button] = isUp;
    
    if (wasLastEventUp && isUp)
    {
        return;
    }
    
    GamepadEvent* e = _poolGamepad.newObject();
    e->_type = isUp ? GET_UP : wasLastEventUp ? GET_DOWN : GET_HELD;
    e->_button = button;
    e->_index = index;
    e->_x = sanitizedX;
    e->_y = sanitizedY;
    
    _poolGamepad.add(e);
    
    if (ENGINE_CFG.inputLoggingEnabled())
    {
        LOG("onGamepadInput %d %d %f %f", button, index, x, y);
    }
}

void InputManager::onKeyboardInput(uint16_t key, bool isUp)
{
    if (!isKeySupported(key))
    {
        if (ENGINE_CFG.inputLoggingEnabled())
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
    
    if (ENGINE_CFG.inputLoggingEnabled())
    {
        LOG("onKeyboardInput %d %d", key, isUp);
    }
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
    assert(_matrix != nullptr);
    
    float w = _matrix->_desc.width();
    float h = _matrix->_desc.height();
    _lastConvertedCursorPos.set((v._x / _cursorWidth) * w, (1 - v._y / _cursorHeight) * h);
    return _lastConvertedCursorPos;
}

void InputManager::setClipboardHandler(ClipboardHandler* clipboardHandler)
{
    _clipboardHandler = clipboardHandler;
}

const char* InputManager::getClipboardString()
{
    assert(_clipboardHandler != nullptr);
    
    return _clipboardHandler->getClipboardString();
}

void InputManager::setCursorSize(int cursorWidth, int cursorHeight)
{
    _cursorWidth = cursorWidth;
    _cursorHeight = cursorHeight;
}

void InputManager::setMatrix(Matrix* m)
{
    _matrix = m;
}

Matrix* InputManager::matrix()
{
    return _matrix;
}

std::string InputManager::mappingForButton(uint8_t button)
{
    auto q = _buttonMap.find(button);
    assert(q != _buttonMap.end());
    
    return q->second;
}

std::string InputManager::mappingForKey(uint16_t key)
{
    auto q = _keyMap.find(key);
    assert(q != _keyMap.end());
    
    return q->second;
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
_clipboardHandler(nullptr),
_matrix(nullptr)
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
    _supportedKeys.push_back(GOW_KEY_NEW_LINE);
    _supportedKeys.push_back(GOW_KEY_BACK_SPACE);
    _supportedKeys.push_back(GOW_KEY_CARRIAGE_RETURN);
    _supportedKeys.push_back(GOW_KEY_ESCAPE);
    _supportedKeys.push_back(GOW_KEY_ANDROID_BACK_BUTTON);
    _supportedKeys.push_back(GOW_KEY_CTRL);
    _supportedKeys.push_back(GOW_KEY_CMD);
    _supportedKeys.push_back(GOW_KEY_DELETE);
    _supportedKeys.push_back(GOW_KEY_ARROW_UP);
    _supportedKeys.push_back(GOW_KEY_ARROW_DOWN);
    _supportedKeys.push_back(GOW_KEY_ARROW_LEFT);
    _supportedKeys.push_back(GOW_KEY_ARROW_RIGHT);
    
    _keyMap[GOW_KEY_0] = "0";
    _keyMap[GOW_KEY_1] = "1";
    _keyMap[GOW_KEY_2] = "2";
    _keyMap[GOW_KEY_3] = "3";
    _keyMap[GOW_KEY_4] = "4";
    _keyMap[GOW_KEY_5] = "5";
    _keyMap[GOW_KEY_6] = "6";
    _keyMap[GOW_KEY_7] = "7";
    _keyMap[GOW_KEY_8] = "8";
    _keyMap[GOW_KEY_9] = "9";
    _keyMap[GOW_KEY_A] = "A";
    _keyMap[GOW_KEY_B] = "B";
    _keyMap[GOW_KEY_C] = "C";
    _keyMap[GOW_KEY_D] = "D";
    _keyMap[GOW_KEY_E] = "E";
    _keyMap[GOW_KEY_F] = "F";
    _keyMap[GOW_KEY_G] = "G";
    _keyMap[GOW_KEY_H] = "H";
    _keyMap[GOW_KEY_I] = "I";
    _keyMap[GOW_KEY_J] = "J";
    _keyMap[GOW_KEY_K] = "K";
    _keyMap[GOW_KEY_L] = "L";
    _keyMap[GOW_KEY_M] = "M";
    _keyMap[GOW_KEY_N] = "N";
    _keyMap[GOW_KEY_O] = "O";
    _keyMap[GOW_KEY_P] = "P";
    _keyMap[GOW_KEY_Q] = "Q";
    _keyMap[GOW_KEY_R] = "R";
    _keyMap[GOW_KEY_S] = "S";
    _keyMap[GOW_KEY_T] = "T";
    _keyMap[GOW_KEY_U] = "U";
    _keyMap[GOW_KEY_V] = "V";
    _keyMap[GOW_KEY_W] = "W";
    _keyMap[GOW_KEY_X] = "X";
    _keyMap[GOW_KEY_Y] = "Y";
    _keyMap[GOW_KEY_Z] = "Z";
    _keyMap[GOW_KEY_NEW_LINE] = "NEW_LINE";
    _keyMap[GOW_KEY_SPACE_BAR] = "SPACE_BAR";
    _keyMap[GOW_KEY_COMMA] = "COMMA";
    _keyMap[GOW_KEY_PERIOD] = "PERIOD";
    _keyMap[GOW_KEY_CTRL] = "CTRL";
    _keyMap[GOW_KEY_CMD] = "CMD";
    _keyMap[GOW_KEY_DELETE] = "DELETE";
    _keyMap[GOW_KEY_BACK_SPACE] = "BACK_SPACE";
    _keyMap[GOW_KEY_ESCAPE] = "ESCAPE";
    _keyMap[GOW_KEY_ANDROID_BACK_BUTTON] = "ANDROID_BACK_BUTTON";
    _keyMap[GOW_KEY_ARROW_LEFT] = "ARROW_LEFT";
    _keyMap[GOW_KEY_ARROW_UP] = "ARROW_UP";
    _keyMap[GOW_KEY_ARROW_RIGHT] = "ARROW_RIGHT";
    _keyMap[GOW_KEY_ARROW_DOWN] = "ARROW_DOWN";
    _keyMap[GOW_KEY_CARRIAGE_RETURN] = "CARRIAGE_RETURN";
    
    _buttonMap[GEB_BUTTON_A] = "A";
    _buttonMap[GEB_BUTTON_B] = "B";
    _buttonMap[GEB_BUTTON_Y] = "Y";
    _buttonMap[GEB_BUTTON_X] = "X";
    _buttonMap[GEB_BUMPER_LEFT] = "BUMPER_LEFT";
    _buttonMap[GEB_BUMPER_RIGHT] = "BUMPER_RIGHT";
    _buttonMap[GEB_BUTTON_BACK] = "BACK";
    _buttonMap[GEB_BUTTON_START] = "START";
    _buttonMap[GEB_BUTTON_GUIDE] = "GUIDE";
    _buttonMap[GEB_D_PAD_UP] = "D_PAD_UP";
    _buttonMap[GEB_D_PAD_RIGHT] = "D_PAD_RIGHT";
    _buttonMap[GEB_D_PAD_DOWN] = "D_PAD_DOWN";
    _buttonMap[GEB_D_PAD_LEFT] = "D_PAD_LEFT";
    _buttonMap[GEB_STICK_LEFT] = "STICK_LEFT";
    _buttonMap[GEB_STICK_RIGHT] = "STICK_RIGHT";
    _buttonMap[GEB_TRIGGER] = "TRIGGER";
}
