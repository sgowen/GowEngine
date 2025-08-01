//
//  InputProcessor.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 10/07/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

InputProcessor::InputProcessor() :
_action(0),
_textInput(""),
_isControlHeldDown(false)
{
    // Empty
}

InputProcessor::~InputProcessor()
{
    // Empty
}

void InputProcessor::registerActionForKey(uint16_t key, uint8_t action)
{
    _keyMappings.emplace(key, action);
}

void InputProcessor::registerActionForButton(uint16_t key, uint8_t action)
{
    _buttonMappings.emplace(key, action);
}

uint8_t InputProcessor::update()
{
    for (KeyboardEvent* e : INPUT_MGR.getKeyboardEvents())
    {
        if (!e->isDown())
        {
            continue;
        }
        
        auto q = _keyMappings.find(e->_key);
        if (q != _keyMappings.end())
        {
            _action = q->second;
            return IPS_ACTION;
        }
        else
        {
            switch (e->_key)
            {
                case GOW_KEY_ESCAPE:
                case GOW_KEY_ANDROID_BACK_BUTTON:
                    return IPS_EXIT;
                default:
                    continue;
            }
        }
    }
    
    for (GamepadEvent* e : INPUT_MGR.getGamepadEvents())
    {
        if (!e->isDown())
        {
            continue;
        }
        
        auto q = _buttonMappings.find(e->_button);
        if (q != _buttonMappings.end())
        {
            _action = q->second;
            return IPS_ACTION;
        }
        else
        {
            switch (e->_button)
            {
                case GPEB_BUTTON_SELECT:
                case GPEB_BUTTON_SNES_SELECT:
                    return IPS_EXIT;
                default:
                    continue;
            }
        }
    }
    
    return IPS_DEFAULT;
}

uint8_t InputProcessor::updateReadText(uint16_t maxInputLength)
{
    for (KeyboardEvent* e : INPUT_MGR.getKeyboardEvents())
    {
        if (e->_key == GOW_KEY_CMD || e->_key == GOW_KEY_CTRL)
        {
            _isControlHeldDown = e->isPressed();
        }
        
        if (!e->isDown())
        {
            continue;
        }
        
        switch (e->_key)
        {
            case GOW_KEY_CARRIAGE_RETURN:
                return IPS_TEXT_INPUT_READY;
            case GOW_KEY_BACK_SPACE:
            case GOW_KEY_DELETE:
            {
                if (_textInput.end() > _textInput.begin())
                {
                    _textInput.erase(_textInput.end() - 1, _textInput.end());
                }
                continue;
            }
            case GOW_KEY_ESCAPE:
                clearTextInput();
                return IPS_EXIT;
            case GOW_KEY_V:
                if (_isControlHeldDown)
                {
                    const char* clipboard = INPUT_MGR.getClipboardString();
                    _textInput += clipboard;
                    if (_textInput.length() >= maxInputLength)
                    {
                        int remove = (int)_textInput.length() - maxInputLength;
                        _textInput.erase(_textInput.end() - remove, _textInput.end());
                    }
                    continue;
                }
            default:
                if (e->_isChar)
                {
                    acceptKeyInput(e->_key);
                }
                continue;
        }
    }
    
    for (GamepadEvent* e : INPUT_MGR.getGamepadEvents())
    {
        if (!e->isDown())
        {
            continue;
        }
        
        switch (e->_button)
        {
            case GPEB_BUTTON_SELECT:
            case GPEB_BUTTON_SNES_SELECT:
                clearTextInput();
                return IPS_EXIT;
            case GPEB_BUTTON_B:
                if (_textInput.end() > _textInput.begin())
                {
                    _textInput.erase(_textInput.end() - 1, _textInput.end());
                }
                continue;
            case GPEB_BUTTON_A:
                acceptKeyInput(GOW_KEY_A);
                continue;
            case GPEB_BUTTON_X:
                acceptKeyInput(GOW_KEY_X);
                continue;
            case GPEB_BUTTON_Y:
            case GPEB_BUMPER_LEFT:
                // Weird that Y button on my SNES controller is coming through as GPEB_BUMPER_LEFT
                acceptKeyInput(GOW_KEY_Y);
                continue;
            case GPEB_BUTTON_START:
            case GPEB_BUTTON_SNES_START:
                return IPS_TEXT_INPUT_READY;
            default:
                continue;
        }
    }
    
    return IPS_DEFAULT;
}

uint8_t InputProcessor::getAction()
{
    return _action;
}

std::string InputProcessor::getTextInput()
{
    return _textInput;
}

void InputProcessor::setTextInput(std::string textInput)
{
    _textInput = textInput;
}

void InputProcessor::clearTextInput()
{
    _textInput.clear();
    _isControlHeldDown = false;
}

void InputProcessor::acceptKeyInput(uint16_t key, uint16_t maxInputLength)
{
    if (_textInput.length() >= maxInputLength)
    {
        return;
    }
    
    char c = static_cast<char>(key);
    _textInput += STRING_FORMAT("%c", c);
}
