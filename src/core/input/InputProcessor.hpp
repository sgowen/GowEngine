//
//  InputProcessor.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 10/07/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

enum InputProcessorState
{
    IPS_DEFAULT,
    IPS_EXIT,
    IPS_ACTION,
    IPS_TEXT_INPUT_READY
};

// TODO This needs to be json driven
// certain touch regions could be defined by name instead of coordinates, a useful feature considering that the button view's size and position would be defined in a renderer.json file
class InputProcessor
{
public:
    InputProcessor();
    virtual ~InputProcessor();
    
    void registerActionForKey(uint16_t key, uint8_t action);
    void registerActionForButton(uint16_t key, uint8_t action);
    
    uint8_t update();
    uint8_t updateReadText();
    
    uint8_t getAction();
    std::string getTextInput();
    void setTextInput(std::string textInput);
    void clearTextInput();
    
private:
    std::map<uint16_t, uint8_t> _keyMappings;
    std::map<uint16_t, uint8_t> _buttonMappings;
    uint8_t _action;
    std::string _textInput;
    bool _isControlHeldDown;
    
    void acceptKeyInput(uint16_t key);
};
