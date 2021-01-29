//
//  Engine.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "Engine.hpp"

#include "EngineController.hpp"
#include "EngineState.hpp"

#include "InputManager.hpp"
#include "PlatformMacros.hpp"
#include "Constants.hpp"
#include "FPSUtil.hpp"

Engine::Engine(EngineController& engineController) :
_stateMachine(this),
_frameRate(FRAME_RATE),
_stateTime(0),
_requestedAction(REQUESTED_ACTION_UPDATE),
_screenWidth(0),
_screenHeight(0),
_cursorWidth(0),
_cursorHeight(0)
{
    _stateMachine.setCurrentState(engineController.getInitialState());
    _stateMachine.getCurrentState()->execute(this);
}

Engine::~Engine()
{
    // Empty
}

void Engine::createDeviceDependentResources()
{
    _stateMachine.getCurrentState()->createDeviceDependentResources();
}

void Engine::onWindowSizeChanged(int screenWidth, int screenHeight, int cursorWidth, int cursorHeight)
{
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;
    _cursorWidth = cursorWidth > 0 ? cursorWidth : screenWidth;
    _cursorHeight = cursorHeight > 0 ? cursorHeight : screenHeight;
    
    _stateMachine.getCurrentState()->onWindowSizeChanged(screenWidth, screenHeight, cursorWidth, cursorHeight);
}

void Engine::releaseDeviceDependentResources()
{
    _stateMachine.getCurrentState()->releaseDeviceDependentResources();
}

void Engine::onResume()
{
    _stateMachine.getCurrentState()->onResume();
}

void Engine::onPause()
{
    _stateMachine.getCurrentState()->onPause();
}

int Engine::update(double deltaTime)
{
    FPS_UTIL.update(deltaTime);
    
    _stateTime += deltaTime;
    while (_stateTime >= _frameRate)
    {
        _stateTime -= _frameRate;
        
        INPUT_MGR.process();
        
        _stateMachine.getCurrentState()->update();
    }
    
    int ret = _requestedAction;
    _requestedAction = REQUESTED_ACTION_UPDATE;
    
    return ret;
}

void Engine::render()
{
    _stateMachine.getCurrentState()->render(_stateTime / _frameRate);
}

void Engine::onCursorDown(float x, float y, bool isAlt)
{
    INPUT_MGR.onCursorInput(CursorEventType_DOWN, x, y, isAlt);
}

void Engine::onCursorDragged(float x, float y, bool isAlt)
{
    INPUT_MGR.onCursorInput(CursorEventType_DRAGGED, x, y, isAlt);
}

void Engine::onCursorMoved(float x, float y)
{
    INPUT_MGR.onCursorInput(CursorEventType_MOVED, x, y);
}

void Engine::onCursorUp(float x, float y, bool isAlt)
{
    INPUT_MGR.onCursorInput(CursorEventType_UP, x, y, isAlt);
}

void Engine::onCursorScrolled(float yoffset)
{
    INPUT_MGR.onCursorInput(CursorEventType_SCROLL, 0, yoffset);
}

void Engine::onGamepadInputStickLeft(uint8_t index, float stickLeftX, float stickLeftY)
{
    INPUT_MGR.onGamepadInput(GamepadEventType_STICK_LEFT, index, stickLeftX, stickLeftY);
}

void Engine::onGamepadInputStickRight(uint8_t index, float stickRightX, float stickRightY)
{
    INPUT_MGR.onGamepadInput(GamepadEventType_STICK_RIGHT, index, stickRightX, stickRightY);
}

void Engine::onGamepadInputTrigger(uint8_t index, float triggerLeft, float triggerRight)
{
    INPUT_MGR.onGamepadInput(GamepadEventType_TRIGGER, index, triggerLeft, triggerRight);
}

void Engine::onGamepadInputButton(uint8_t index, uint8_t gamepadEventType, uint8_t isPressed)
{
    INPUT_MGR.onGamepadInput((GamepadEventType)gamepadEventType, index, isPressed);
}

void Engine::onKeyboardInput(unsigned short key, bool isUp)
{
    INPUT_MGR.onKeyboardInput(key, isUp);
}

void Engine::setRequestedAction(int value)
{
    _requestedAction = value;
}

StateMachine<Engine, EngineState>& Engine::getStateMachine()
{
    return _stateMachine;
}

int Engine::screenWidth()
{
    return _screenWidth;
}

int Engine::screenHeight()
{
    return _screenHeight;
}

int Engine::cursorWidth()
{
    return _cursorWidth;
}

int Engine::cursorHeight()
{
    return _cursorHeight;
}
