//
//  DefaultEngineState.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "EngineState.hpp"

class DefaultEngineState : public EngineState
{
public:
    static DefaultEngineState& getInstance()
    {
        static DefaultEngineState ret = DefaultEngineState();
        return ret;
    }

protected:
    virtual void onEnter(Engine* e) override;
    virtual void onAssetsLoaded(Engine* e) override;
    virtual void onExit(Engine* e) override;
    virtual void onUpdate(Engine* e) override;
    virtual void onRender(Renderer& r) override;
    
private:
    bool needsToRenderModePicker;

    DefaultEngineState();
    virtual ~DefaultEngineState() {}
    DefaultEngineState(const DefaultEngineState&);
    DefaultEngineState& operator=(const DefaultEngineState&);
};
