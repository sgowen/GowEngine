//
//  EngineState.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "EngineState.hpp"

#include "Engine.hpp"
#include "deps/rapidjson/AssetsLoader.hpp"
#include "deps/rapidjson/RendererLoader.hpp"
#include "AssetsManager.hpp"
#include "AudioEngineFactory.hpp"

void EngineState::enter(Engine* e)
{
    ASSETS.registerAssets(_assetsFilePath, _assets);
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
        case ERSA_RELEASE_RESOURCES:
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
    ASSETS.deregisterAssets(_assetsFilePath);
    onExit(e);
}

EngineState::EngineState(std::string assetsFilePath, std::string rendererFilePath, RenderFunc rf) : State<Engine>(),
_assets(AssetsLoader::initWithJSONFile(assetsFilePath)),
_renderer(RendererLoader::initWithJSONFile(rendererFilePath)),
_assetsFilePath(assetsFilePath)
{
    _renderer.setRenderFunc(rf);
}

void EngineState::createDeviceDependentResources(Engine* e)
{
    ASSETS.createDeviceDependentResourcesAsync();
    _renderer.createDeviceDependentResources();
}

void EngineState::onWindowSizeChanged(Engine* e)
{
    _renderer.onWindowSizeChanged(e->screenWidth(), e->screenHeight());
}

void EngineState::destroyDeviceDependentResources(Engine* e)
{
    _renderer.destroyDeviceDependentResources();
    ASSETS.destroyDeviceDependentResources();
}

void EngineState::pause(Engine *e)
{
    AUDIO_ENGINE.pause();
}

void EngineState::resume(Engine *e)
{
    AUDIO_ENGINE.resume();
}

void EngineState::update(Engine* e)
{
    ASSETS.update();
    if (!ASSETS.isLoaded())
    {
        return;
    }
    
    onUpdate(e);
}

void EngineState::render(Engine* e)
{
    if (!ASSETS.isLoaded())
    {
        _renderer.renderLoadingScreen();
        return;
    }
    
    _renderer.render();
    AUDIO_ENGINE.render();
}
