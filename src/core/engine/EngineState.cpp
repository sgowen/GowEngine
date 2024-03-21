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
    createDeviceDependentResources(e);
    
    onEnter(e);
}

void EngineState::execute(Engine* e)
{
    bool areAssetsLoaded = ASSETS_MGR.isLoaded();
    
    ASSETS_MGR.update();
    
    if (!areAssetsLoaded && ASSETS_MGR.isLoaded())
    {
        Renderer& renderer = ASSETS_MGR.renderer(_filePathAssets);
        INPUT_MGR.setMatrix(&renderer.matrixForInput());
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
    onExit(e);
    
    if (!_areAssetsGlobal)
    {
        destroyDeviceDependentResources(e);
    }
}

void EngineState::createDeviceDependentResources(Engine* e)
{
    ASSETS_MGR.registerAssets(_filePathAssets);
    ASSETS_MGR.createDeviceDependentResourcesAsync();
}

void EngineState::destroyDeviceDependentResources(Engine* e)
{
    ASSETS_MGR.destroyDeviceDependentResources();
    ASSETS_MGR.deregisterAssets(_filePathAssets);
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
        if (ASSETS_MGR.isRendererLoaded(FILE_PATH_ENGINE_ASSETS))
        {
            Renderer& renderer = ASSETS_MGR.renderer(FILE_PATH_ENGINE_ASSETS);
            renderer.renderLoadingView(ASSETS_MGR.getStateTime());
        }
        
        return;
    }
    
    Renderer& renderer = ASSETS_MGR.renderer(_filePathAssets);
    
    renderer.configReset();
    renderer.configExtrapolation(e->extrapolation());
    renderer.configScreenSize(e->screenWidth(), e->screenHeight());
    onRender(renderer);
    
    AUDIO_ENGINE.render();
}

EngineState::EngineState(std::string filePathAssets, bool areAssetsGlobal) : State<Engine>(),
_filePathAssets(filePathAssets),
_areAssetsGlobal(areAssetsGlobal)
{
    // Empty
}
