//
//  EngineState.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "EngineState.hpp"

#include "Engine.hpp"
#include "rapidjson/AssetsLoader.hpp"
#include "rapidjson/RendererLoader.hpp"
#include "AssetsManager.hpp"
#include "AudioEngineFactory.hpp"

void EngineState::enter(Engine* e)
{
    ASSETS.registerAssets(_assetsFilePath, _assets);
    createDeviceDependentResources(e);
    onWindowSizeChanged(e);
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
            releaseDeviceDependentResources(e);
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
    releaseDeviceDependentResources(e);
    ASSETS.deregisterAssets(_assetsFilePath);
}

void EngineState::createDeviceDependentResources(Engine* e)
{
    ASSETS.createDeviceDependentResources();
    _renderer.createDeviceDependentResources();
}

void EngineState::onWindowSizeChanged(Engine* e)
{
    _renderer.onWindowSizeChanged(e->screenWidth(), e->screenHeight());
}

void EngineState::releaseDeviceDependentResources(Engine* e)
{
    _renderer.releaseDeviceDependentResources();
    ASSETS.releaseDeviceDependentResources();
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
    // TODO, call ASSETS.update to handle async callbacks
}

void EngineState::render(Engine* e)
{
    AUDIO_ENGINE.render();
}

EngineState::EngineState(std::string assetsFilePath, std::string rendererFilePath) : State<Engine>(),
_assets(AssetsLoader::initWithJSONFile(assetsFilePath.c_str())),
_renderer(RendererLoader::initWithJSONFile(rendererFilePath.c_str())),
_assetsFilePath(assetsFilePath)
{
    // Empty
}
