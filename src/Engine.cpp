//
//  Engine.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "Engine.hpp"

#include "EngineController.hpp"

#include "InputManager.hpp"
#include "FPSUtil.hpp"
#include "ClipboardUtil.hpp"
#include "PlatformMacros.hpp"

#if IS_ANDROID
    #include "android/JNIAndroidAssetHandler.hpp"
#endif

Engine::Engine(EngineController& ec) :
_stateMachine(this, ec.getInitialState()),
_requestedStateAction(ERSA_DEFAULT),
_requestedHostAction(ERHA_DEFAULT),
_frameRate(ec.getFrameRate()),
_stateTime(0),
_screenWidth(0),
_screenHeight(0),
_cursorWidth(0),
_cursorHeight(0)
{
    // Empty
}

void Engine::createDeviceDependentResources(void* data)
{
//#if IS_DESKTOP
    CLIPBOARD_UTIL.init(data);
//#elif IS_ANDROID
//    JNIAndroidAssetHandler::create(data);
//#endif
    
    execute(ERSA_CREATE_RESOURCES);
}

void Engine::onWindowSizeChanged(int screenWidth, int screenHeight, int cursorWidth, int cursorHeight)
{
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;
    _cursorWidth = cursorWidth > 0 ? cursorWidth : screenWidth;
    _cursorHeight = cursorHeight > 0 ? cursorHeight : screenHeight;
    
    INPUT_MGR.setCursorSize(_cursorWidth, _cursorHeight);
    
    execute(ERSA_WINDOW_SIZE_CHANGED);
}

void Engine::releaseDeviceDependentResources()
{
    execute(ERSA_RELEASE_RESOURCES);
    
#if IS_ANDROID
//    JNIAndroidAssetHandler::destroy();
#endif
}

void Engine::onResume()
{
    execute(ERSA_RESUME);
}

void Engine::onPause()
{
    execute(ERSA_PAUSE);
}

EngineRequestedHostAction Engine::update(double deltaTime)
{
    FPS_UTIL.update(deltaTime);
    
    _stateTime += deltaTime;
    while (_stateTime >= _frameRate)
    {
        _stateTime -= _frameRate;
        
        INPUT_MGR.process();
        
        execute(ERSA_UPDATE);
    }
    
    EngineRequestedHostAction ret = _requestedHostAction;
    _requestedHostAction = ERHA_DEFAULT;
    
    return ret;
}

void Engine::render()
{
    execute(ERSA_RENDER);
}

void Engine::onCursorDown(float x, float y, bool isAlt)
{
    INPUT_MGR.onCursorInput(CUET_DOWN, x, y, isAlt);
}

void Engine::onCursorDragged(float x, float y, bool isAlt)
{
    INPUT_MGR.onCursorInput(CUET_DRAGGED, x, y, isAlt);
}

void Engine::onCursorMoved(float x, float y)
{
    INPUT_MGR.onCursorInput(CUET_MOVED, x, y);
}

void Engine::onCursorUp(float x, float y, bool isAlt)
{
    INPUT_MGR.onCursorInput(CUET_UP, x, y, isAlt);
}

void Engine::onCursorScrolled(float yoffset)
{
    INPUT_MGR.onCursorInput(CUET_SCROLL, 0, yoffset);
}

void Engine::onGamepadInputStickLeft(uint8_t index, float stickLeftX, float stickLeftY)
{
    INPUT_MGR.onGamepadInput(GPEB_STICK_LEFT, index, stickLeftX, stickLeftY);
}

void Engine::onGamepadInputStickRight(uint8_t index, float stickRightX, float stickRightY)
{
    INPUT_MGR.onGamepadInput(GPEB_STICK_RIGHT, index, stickRightX, stickRightY);
}

void Engine::onGamepadInputTrigger(uint8_t index, float triggerLeft, float triggerRight)
{
    INPUT_MGR.onGamepadInput(GPEB_TRIGGER, index, triggerLeft, triggerRight);
}

void Engine::onGamepadInputButton(uint8_t index, uint8_t button, uint8_t isPressed)
{
    INPUT_MGR.onGamepadInput(button, index, isPressed);
}

void Engine::onKeyboardInput(uint16_t key, bool isUp)
{
    INPUT_MGR.onKeyboardInput(key, isUp);
}

void Engine::setRequestedHostAction(EngineRequestedHostAction value)
{
    _requestedHostAction = value;
}

StateMachine<Engine>& Engine::getStateMachine()
{
    return _stateMachine;
}

EngineRequestedStateAction Engine::state()
{
    return _requestedStateAction;
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

void Engine::execute(EngineRequestedStateAction ersa)
{
    _requestedStateAction = ersa;
    _stateMachine.execute();
    _requestedStateAction = ERSA_DEFAULT;
}
