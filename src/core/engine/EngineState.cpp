//
//  EngineState.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
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
    ASSETS_MGR.deregisterAssets(_filePathAssets);
    destroyDeviceDependentResources(e);
    onExit(e);
}

EngineState::EngineState(std::string configFilePath) : State<Engine>()
{
    // Yeah, this should all should be loaded async.
    
    ConfigLoader::initWithJSONFile(_config, configFilePath);
    
    _filePathAssets = _config.getString("filePathAssets");
    
    AssetsLoader::initWithJSONFile(_assets, _filePathAssets);
    RendererLoader::initWithJSONFile(_renderer, _config.getString("filePathRenderer"));
    
    INPUT_MGR.setMatrix(&_renderer.matrix());
}

void EngineState::createDeviceDependentResources(Engine* e)
{
    ASSETS_MGR.createDeviceDependentResourcesAsync();
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
    bool areAssetsLoaded = ASSETS_MGR.isLoaded();
    
    ASSETS_MGR.update();
    
    if (!areAssetsLoaded)
    {
        if (ASSETS_MGR.isLoaded())
        {
            onAssetsLoaded(e);
        }
        
        return;
    }
    
    onUpdate(e);
}

void EngineState::render(Engine* e)
{
    if (!ASSETS_MGR.isLoaded())
    {
        _renderer.renderLoadingView(ASSETS_MGR.getStateTime());
        return;
    }
    
    onRender(_renderer, e->extrapolation());
    AUDIO_ENGINE.render();
}
