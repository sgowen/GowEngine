//
//  Engine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

enum EngineRequestedHostAction
{
    EngineRequestedHostAction_NONE =    0,
    EngineRequestedHostAction_EXIT =    1 << 0
};

#include "StateMachine.hpp"

#include <stdint.h>

class EngineController;
class EngineState;

class Engine
{
public:
    Engine(EngineController& engineController);
    ~Engine() {}
    
    void createDeviceDependentResources();
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
    void onKeyboardInput(unsigned short key, bool isUp = false);
    void setRequestedAction(EngineRequestedHostAction value);
    StateMachine<Engine, EngineState>& getStateMachine();
    int screenWidth();
    int screenHeight();
    int cursorWidth();
    int cursorHeight();
    
private:
    StateMachine<Engine, EngineState> _stateMachine;
    double _frameRate;
    double _stateTime;
    EngineRequestedHostAction _requestedAction;
    int _screenWidth;
    int _screenHeight;
    int _cursorWidth;
    int _cursorHeight;
};
