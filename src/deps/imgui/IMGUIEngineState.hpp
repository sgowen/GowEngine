//
//  IMGUIEngineState.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

#include "EngineState.hpp"

#include "core/input/InputProcessor.hpp"

class IMGUIEngineState : public EngineState
{
public:
    static IMGUIEngineState& getInstance()
    {
        static IMGUIEngineState ret = IMGUIEngineState();
        return ret;
    }
    
    virtual void onEnter(Engine* e);
    virtual void onAssetsLoaded(Engine* e);
    virtual void onExit(Engine* e);
    virtual void onUpdate(Engine* e);
    virtual void onRender(Renderer& r);
    
private:
    InputProcessor _inputProcessor;
    
    IMGUIEngineState();
    virtual ~IMGUIEngineState() {}
    IMGUIEngineState(const IMGUIEngineState&);
    IMGUIEngineState& operator=(const IMGUIEngineState&);
};
