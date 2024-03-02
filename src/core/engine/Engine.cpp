//
//  Engine.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

Engine::Engine() :
_stateMachine(this, &ENGINE_STATE_DEFAULT),
_renderer(new Renderer()),
_requestedStateAction(ERSA_DEFAULT),
_requestedHostAction(ERHA_DEFAULT),
_stateTime(0.0),
_screenWidth(0),
_screenHeight(0),
_cursorWidth(0),
_cursorHeight(0),
_hasUpdatedSinceLastRender(false)
{
    EngineConfig::create("engine/json/Config.json");
    
    if (ENGINE_CFG.fileLoggingEnabled())
    {
        std::string cacheDir = CACHE_FILE_DIR;
        std::string logFileName = STRING_FORMAT("%s/log_GowEngine_%d", cacheDir.c_str(), TimeUtil::timeSinceEpochMillisec());
        Logger::getInstance().initWithFile(logFileName.c_str());
    }
    
    Assets assets;
    AssetsLoader::initWithJSONFile(assets, "engine/json/Assets.json");
    ASSETS_MGR.registerAssets(ENGINE_ASSETS, assets);
    
    RendererLoader::initWithJSONFile(*_renderer, "engine/json/Renderer.json");
}

Engine::~Engine()
{
    delete _renderer;
    
    ASSETS_MGR.deregisterAssets(ENGINE_ASSETS);
    
    Logger::getInstance().closeFileStream();
    
#if IS_DESKTOP
    if (ENGINE_CFG.useSteamNetworking())
    {
        SteamGameServices::destroy();
    }
#endif
    
    EngineConfig::destroy();
}

void Engine::createDeviceDependentResources(ClipboardHandler* clipboardHandler)
{
    INPUT_MGR.setClipboardHandler(clipboardHandler);
    
    _renderer->createDeviceDependentResources();
    
    AudioEngine::create();
    
    execute(ERSA_CREATE_RESOURCES);
}

void Engine::onWindowSizeChanged(uint16_t screenWidth, uint16_t screenHeight, uint16_t cursorWidth, uint16_t cursorHeight)
{
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;
    _cursorWidth = cursorWidth > 0 ? cursorWidth : screenWidth;
    _cursorHeight = cursorHeight > 0 ? cursorHeight : screenHeight;
    
    _renderer->onWindowSizeChanged(screenWidth, screenHeight);
    INPUT_MGR.setCursorSize(_cursorWidth, _cursorHeight);
    
    execute(ERSA_WINDOW_SIZE_CHANGED);
}

void Engine::destroyDeviceDependentResources()
{
    _renderer->destroyDeviceDependentResources();
    
    AudioEngine::destroy();
    
    execute(ERSA_DESTROY_RESOURCES);
}

void Engine::onPause()
{
    execute(ERSA_PAUSE);
}

void Engine::onResume()
{
    execute(ERSA_RESUME);
}

EngineRequestedHostAction Engine::update(double deltaTime)
{
    static double frameRate = ENGINE_CFG.frameRate();
    static double maxStateTimeAllowed = frameRate * 3;
    
    FPS_UTIL.update(deltaTime);
    
    _stateTime += deltaTime;
    if (_stateTime > maxStateTimeAllowed)
    {
        LOG("Okay! So this code really is here for a reason");
        _stateTime = frameRate;
    }
    
    while (_stateTime >= frameRate)
    {
        _stateTime -= frameRate;
        
        // Better to reuse input for the additional update
        // than have a frame where there is no input at all
        if (_hasUpdatedSinceLastRender)
        {
            if (ENGINE_CFG.inputLoggingEnabled())
            {
                LOG("Reusing input from last frame");
            }
        }
        else
        {
            INPUT_MGR.process();
        }
        
        FPS_UTIL.onFrame();
        execute(ERSA_UPDATE);
        _hasUpdatedSinceLastRender = true;
    }
    
    EngineRequestedHostAction ret = _requestedHostAction;
    _requestedHostAction = ERHA_DEFAULT;
    
    return ret;
}

void Engine::render()
{
    execute(ERSA_RENDER);
    _hasUpdatedSinceLastRender = false;
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

void Engine::overwriteState(State<Engine>* state, const Config& args)
{
    _stateMachine.overwriteState(state, args);
}

void Engine::pushState(State<Engine>* state, const Config& args)
{
    _stateMachine.pushState(state, args);
}

void Engine::popState()
{
    if (_stateMachine.isOnLastState())
    {
        setRequestedHostAction(ERHA_EXIT);
        return;
    }
    
    _stateMachine.popState();
    
    if (_stateMachine.getCurrentState() == &DefaultEngineState::getInstance())
    {
        // We are back in the default engine state
        // This is only possible when running GowEngine
        // with mode set to "engine"
        ENGINE_CFG.mode() = "engine";
        INPUT_MGR.setMatrix(&_renderer->matrixForInput());
    }
}

void Engine::setRequestedHostAction(EngineRequestedHostAction value)
{
    _requestedHostAction = value;
}

EngineRequestedStateAction Engine::requestedStateAction()
{
    return _requestedStateAction;
}

uint16_t Engine::screenWidth()
{
    return _screenWidth;
}

uint16_t Engine::screenHeight()
{
    return _screenHeight;
}

uint16_t Engine::cursorWidth()
{
    return _cursorWidth;
}

uint16_t Engine::cursorHeight()
{
    return _cursorHeight;
}

double Engine::extrapolation()
{
    return _stateTime;
}

bool Engine::hasUpdatedSinceLastRender()
{
    return _hasUpdatedSinceLastRender;
}

void Engine::execute(EngineRequestedStateAction ersa)
{
    _requestedStateAction = ersa;
    _stateMachine.execute();
    _requestedStateAction = ERSA_DEFAULT;
}

void Engine::renderModePicker()
{
    _renderer->bindFramebuffer("main");
    _renderer->renderTextViews();
    _renderer->renderToScreen();
}
