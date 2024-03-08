//
//  DefaultEngineState.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void DefaultEngineState::enter(Engine *e)
{
    ASSETS_MGR.createDeviceDependentResources();
}

void DefaultEngineState::execute(Engine* e)
{
    switch (e->requestedStateAction())
    {
        case ERSA_CREATE_RESOURCES:
            ERSA_CREATE_RESOURCES_called = true;
            ASSETS_MGR.createDeviceDependentResources();
            break;
        case ERSA_WINDOW_SIZE_CHANGED:
            ERSA_WINDOW_SIZE_CHANGED_called = true;
            break;
        case ERSA_DESTROY_RESOURCES:
            ASSETS_MGR.destroyDeviceDependentResources();
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
    
    if (ERSA_CREATE_RESOURCES_called && ERSA_WINDOW_SIZE_CHANGED_called)
    {
#if IS_DESKTOP
        if (ENGINE_CFG.useSteamNetworking())
        {
            SteamGameServices::create(ENGINE_CFG.steamGameDir().c_str());
        }
#endif
        
        if (needsToRenderModePicker)
        {
            return;
        }
        
        bool isEngineMode = strcmp(ENGINE_CFG.mode().c_str(), "engine") == 0;
        if (isEngineMode)
        {
            needsToRenderModePicker = true;
        }
    }
}

void DefaultEngineState::update(Engine* e)
{
    ASSETS_MGR.update();
    if (!ASSETS_MGR.isLoaded())
    {
        return;
    }
    
    if (!needsToRenderModePicker)
    {
        e->overwriteState(&ENGINE_STATE_TITLE);
        return;
    }
    
#if IS_MOBILE
    // TODO
#endif
        
    for (KeyboardEvent* kbe : INPUT_MGR.getKeyboardEvents())
    {
        if (!kbe->isDown())
        {
            continue;
        }
        
        switch (kbe->_key)
        {
            case GOW_KEY_ESCAPE:
            case GOW_KEY_ANDROID_BACK_BUTTON:
                e->popState();
                return;
            case GOW_KEY_D:
                ENGINE_CFG.mode() = "dante";
                e->pushState(&ENGINE_STATE_TITLE);
                return;
            case GOW_KEY_N:
                ENGINE_CFG.mode() = "nosfuratu";
                e->pushState(&ENGINE_STATE_TITLE);
                return;
            default:
                break;
        }
    }
}

void DefaultEngineState::render(Engine* e)
{
    if (!needsToRenderModePicker ||
        !ASSETS_MGR.isLoaded())
    {
        return;
    }
    
    e->renderModePicker();
}

DefaultEngineState::DefaultEngineState() : State<Engine>(),
ERSA_CREATE_RESOURCES_called(false),
ERSA_WINDOW_SIZE_CHANGED_called(false),
needsToRenderModePicker(false)
{
    // Empty
}
