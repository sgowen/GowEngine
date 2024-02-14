//
//  Engine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/StateMachine.hpp"
#include "core/input/ClipboardHandler.hpp"
#include "EngineConfig.hpp"

#include <stdint.h>

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

class Config;
class EngineState;

class Engine
{
    friend class DefaultEngineState;
    
public:
    Engine();
    ~Engine();
    
    void createDeviceDependentResources(ClipboardHandler* clipboardHandler = &NullClipboardHandler::getInstance());
    void onWindowSizeChanged(uint16_t screenWidth, uint16_t screenHeight, uint16_t cursorWidth = 0, uint16_t cursorHeight = 0);
    void destroyDeviceDependentResources();
    void onPause();
    void onResume();
    EngineRequestedHostAction update(double deltaTime);
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
    void overwriteState(State<Engine>* state, const Config& args = Config::EMPTY);
    void pushState(State<Engine>* state, const Config& args = Config::EMPTY);
    void popState();
    void setRequestedHostAction(EngineRequestedHostAction value);
    EngineRequestedStateAction requestedStateAction();
    uint16_t screenWidth();
    uint16_t screenHeight();
    uint16_t cursorWidth();
    uint16_t cursorHeight();
    double extrapolation();
    bool hasUpdatedSinceLastRender();
    
private:
    StateMachine<Engine> _stateMachine;
    EngineRequestedStateAction _requestedStateAction;
    EngineRequestedHostAction _requestedHostAction;
    double _stateTime;
    double _extrapolation;
    uint16_t _screenWidth;
    uint16_t _screenHeight;
    uint16_t _cursorWidth;
    uint16_t _cursorHeight;
    bool _hasUpdatedSinceLastRender;
    
    void execute(EngineRequestedStateAction ersa);
};
