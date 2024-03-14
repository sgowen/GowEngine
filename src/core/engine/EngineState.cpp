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
    ConfigLoader::initWithJSONFile(_config, _configFilePath);
    
    AssetsLoader::initWithJSONFile(_assets, _filePathAssets);
    RendererLoader::initWithJSONFile(_renderer, _config.getString("filePathRenderer"));
    
    INPUT_MGR.setMatrix(&_renderer.matrixForInput());
    
    ASSETS_MGR.registerAssets(_filePathAssets, _assets);
    createDeviceDependentResources(e);
    onWindowSizeChanged(e);
    onEnter(e);
}

void EngineState::execute(Engine* e)
{
    bool areAssetsLoaded = ASSETS_MGR.isLoaded();
    
    ASSETS_MGR.update();
    
    if (!areAssetsLoaded && ASSETS_MGR.isLoaded())
    {
        onAssetsLoaded(e);
        areAssetsLoaded = true;
    }
    
    if (!areAssetsLoaded)
    {
        return;
    }
    
    onUpdate(e);
}

void EngineState::exit(Engine* e)
{
    ASSETS_MGR.deregisterAssets(_filePathAssets);
    destroyDeviceDependentResources(e);
    onExit(e);
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

void EngineState::render(Engine* e)
{
    if (!ASSETS_MGR.isLoaded())
    {
        _renderer.renderLoadingView(ASSETS_MGR.getStateTime());
        return;
    }
    
    _renderer.configReset();
    _renderer.configExtrapolation(e->extrapolation());
    onRender(_renderer);
    AUDIO_ENGINE.render();
}

EngineState::EngineState(std::string configFilePath, std::string filePathAssets) : State<Engine>()
{
    _configFilePath = configFilePath;
    _filePathAssets = filePathAssets;
}
