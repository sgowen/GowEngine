//
//  DefaultEngineState.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void DefaultEngineState::onEnter(Engine* e)
{
    // Empty
}

void DefaultEngineState::onAssetsLoaded(Engine *e)
{
    // Empty
}

void DefaultEngineState::onExit(Engine* e)
{
    // Empty
}

void DefaultEngineState::onUpdate(Engine* e)
{
    bool isEngineMode = strcmp(ENGINE_CFG.mode().c_str(), "engine") == 0;
    
    if (!isEngineMode)
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
            case GOW_KEY_C:
                // TODO, create new mode!
                return;
            case GOW_KEY_D:
                ENGINE_CFG.mode() = "dante";
                ENGINE_CFG.physicsEngine() = "Box2D";
                e->pushState(&ENGINE_STATE_TITLE);
                return;
            case GOW_KEY_N:
                ENGINE_CFG.mode() = "nosfuratu";
                ENGINE_CFG.physicsEngine() = "Nos";
                e->pushState(&ENGINE_STATE_TITLE);
                return;
            default:
                break;
        }
    }
}

void DefaultEngineState::onRender(Renderer& r)
{
    // TODO, render using engine embedded Lua script
//    e->renderModePicker();
}

DefaultEngineState::DefaultEngineState() : EngineState(ENGINE_ASSETS, true)
{
    // Empty
}
