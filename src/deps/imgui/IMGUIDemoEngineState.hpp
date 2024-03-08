//
//  IMGUIDemoEngineState.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildConstants.hpp>
#if IS_DESKTOP

#include "core/engine/EngineState.hpp"

#include "core/input/InputProcessor.hpp"

class IMGUIDemoEngineState : public EngineState
{
public:
    static IMGUIDemoEngineState& getInstance()
    {
        static IMGUIDemoEngineState ret = IMGUIDemoEngineState();
        return ret;
    }
    
    virtual void onEnter(Engine* e);
    virtual void onAssetsLoaded(Engine* e);
    virtual void onExit(Engine* e);
    virtual void onUpdate(Engine* e);
    virtual void onRender(Renderer& r);
    
private:
    InputProcessor _inputProcessor;
    
    IMGUIDemoEngineState();
    virtual ~IMGUIDemoEngineState() {}
    IMGUIDemoEngineState(const IMGUIDemoEngineState&);
    IMGUIDemoEngineState& operator=(const IMGUIDemoEngineState&);
};

#endif /* IS_DESKTOP */
