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
    ERSA_RELEASE_RESOURCES,
    ERSA_RESUME,
    ERSA_PAUSE,
    ERSA_UPDATE,
    ERSA_RENDER
};

enum EngineRequestedHostAction
{
    ERHA_DEFAULT,
    ERHA_EXIT
};

#include "StateMachine.hpp"

#include <stdint.h>

class EngineController;

class Engine
{
public:
    Engine(EngineController& engineController);
    ~Engine() {}
    
    void createDeviceDependentResources(void* data = NULL);
    void onWindowSizeChanged(int screenWidth, int screenHeight, int cursorWidth = 0, int cursorHeight = 0);
    void releaseDeviceDependentResources();
    void onResume();
    void onPause();
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
    void setRequestedHostAction(EngineRequestedHostAction value);
    StateMachine<Engine>& getStateMachine();
    EngineRequestedStateAction state();
    int screenWidth();
    int screenHeight();
    int cursorWidth();
    int cursorHeight();
    
private:
    StateMachine<Engine> _stateMachine;
    EngineRequestedStateAction _requestedStateAction;
    EngineRequestedHostAction _requestedHostAction;
    double _frameRate;
    double _stateTime;
    int _screenWidth;
    int _screenHeight;
    int _cursorWidth;
    int _cursorHeight;
    
    void execute(EngineRequestedStateAction ersa);
};
