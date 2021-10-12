//
//  Engine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

enum EngineRequestedStateAction
{
    ERSA_DEFAULT,
    ERSA_CREATE_RESOURCES,
    ERSA_WINDOW_SIZE_CHANGED,
    ERSA_DESTROY_RESOURCES,
    ERSA_PAUSE,
    ERSA_RESUME,
    ERSA_UPDATE,
    ERSA_RENDER
};

enum EngineRequestedHostAction
{
    ERHA_DEFAULT,
    ERHA_EXIT
};

#include "core/common/StateMachine.hpp"
#include "core/input/ClipboardHandler.hpp"

#include <stdint.h>

class EngineConfig;
class Config;

class Engine
{
    friend class DefaultEngineState;
    
public:
    Engine(EngineConfig& engineConfig, State<Engine>& initialEngineState);
    
    void createDeviceDependentResources(ClipboardHandler* clipboardHandler = &NullClipboardHandler::getInstance());
    void onWindowSizeChanged(uint16_t screenWidth, uint16_t screenHeight, uint16_t cursorWidth = 0, uint16_t cursorHeight = 0);
    void destroyDeviceDependentResources();
    void onPause();
    void onResume();
    EngineRequestedHostAction update(float deltaTime);
    void render();
    
    void onCursorDown(float x, float y, bool isAlt = false);
    void onCursorDragged(float x, float y, bool isAlt = false);
    void onCursorMoved(float x, float y);
    void onCursorUp(float x, float y, bool isAlt = false);
    void onCursorScrolled(float yoffset);
    void onGamepadInputStickLeft(uint8_t index, float stickLeftX, float stickLeftY);
    void onGamepadInputStickRight(uint8_t index, float stickRightX, float stickRightY);
    void onGamepadInputTrigger(uint8_t index, float triggerLeft, float triggerRight);
    void onGamepadInputButton(uint8_t index, uint8_t gamepadEventType, uint8_t isPressed);
    void onKeyboardInput(uint16_t key, bool isUp);
    void changeState(State<Engine>* state, const Config& args = Config::EMPTY);
    void revertToPreviousState();
    void setRequestedHostAction(EngineRequestedHostAction value);
    EngineRequestedStateAction requestedStateAction();
    uint16_t screenWidth();
    uint16_t screenHeight();
    uint16_t cursorWidth();
    uint16_t cursorHeight();
    
private:
    State<Engine>& _initialState;
    StateMachine<Engine> _stateMachine;
    EngineRequestedStateAction _requestedStateAction;
    EngineRequestedHostAction _requestedHostAction;
    float _frameRate;
    float _stateTime;
    uint16_t _screenWidth;
    uint16_t _screenHeight;
    uint16_t _cursorWidth;
    uint16_t _cursorHeight;
    
    void execute(EngineRequestedStateAction ersa);
};
