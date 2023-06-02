//
//  EngineState.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void EngineState::enter(Engine* e)
{
    // Should be able to load everything asynchronously, including the config
    ASSETS_MGR.registerAssets(_filePathAssets, _assets);
    createDeviceDependentResources(e);
    onWindowSizeChanged(e);
    onEnter(e);
}

void EngineState::execute(Engine* e)
{
    switch (e->requestedStateAction())
    {
        case ERSA_CREATE_RESOURCES:
            createDeviceDependentResources(e);
            break;
        case ERSA_WINDOW_SIZE_CHANGED:
            onWindowSizeChanged(e);
            break;
        case ERSA_DESTROY_RESOURCES:
            destroyDeviceDependentResources(e);
            break;
        case ERSA_PAUSE:
            pause(e);
            break;
        case ERSA_RESUME:
            resume(e);
            break;
        case ERSA_UPDATE:
            update(e);
            break;
        case ERSA_RENDER:
            render(e);
            break;
        case ERSA_DEFAULT:
        default:
            break;
    }
}

void EngineState::exit(Engine* e)
{
    destroyDeviceDependentResources(e);
    ASSETS_MGR.deregisterAssets(_filePathAssets);
    onExit(e);
}

EngineState::EngineState(std::string configFilePath) : State<Engine>(),
// Yeah, this config should be loaded async.
_config(ConfigLoader::initWithJSONFile(configFilePath)),
_stateTime(0),
_filePathAssets(_config.getString("filePathAssets")),
_assets(AssetsLoader::initWithJSONFile(_filePathAssets)),
_renderer(RendererLoader::initWithJSONFile(_config.getString("filePathRenderer")))
{
    INPUT_MGR.setMatrix(&_renderer.matrix());
}

void EngineState::createDeviceDependentResources(Engine* e)
{
    ASSETS_MGR.createDeviceDependentResources();
    _renderer.createDeviceDependentResources();
}

void EngineState::onWindowSizeChanged(Engine* e)
{
    _renderer.onWindowSizeChanged(e->screenWidth(), e->screenHeight());
}

void EngineState::destroyDeviceDependentResources(Engine* e)
{
    _renderer.destroyDeviceDependentResources();
    ASSETS_MGR.destroyDeviceDependentResources();
}

void EngineState::pause(Engine* e)
{
    AUDIO_ENGINE.pause();
}

void EngineState::resume(Engine* e)
{
    AUDIO_ENGINE.resume();
}

void EngineState::update(Engine* e)
{
    ASSETS_MGR.update();
    
    onUpdate(e);
    ++_stateTime;
}

void EngineState::render(Engine* e)
{
    if (!ASSETS_MGR.isLoaded())
    {
        _renderer.renderLoadingView();
        return;
    }
    
    onRender(_renderer);
    AUDIO_ENGINE.render();
}
