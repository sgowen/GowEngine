//
//  TitleEngineState.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 9/29/21.
//  Copyright © 2020 Stephen Gowen. All rights reserved.
//

#pragma once

#include "EngineState.hpp"

#include "core/input/InputProcessor.hpp"

class TitleEngineState : public EngineState
{
public:
    enum TitleEngineStateState
    {
        TESS_DEFAULT,
        TESS_INPUT_HOST_NAME,
        TESS_INPUT_IP,
        TESS_INPUT_JOIN_NAME,
        TESS_START_OFFLINE
    };
    
    static TitleEngineState& getInstance()
    {
        static TitleEngineState ret = TitleEngineState();
        return ret;
    }
    
    virtual void onEnter(Engine* e);
    virtual void onAssetsLoaded(Engine* e);
    virtual void onExit(Engine* e);
    virtual void onUpdate(Engine* e);
    virtual void onRender(Renderer& r, double extrapolation);
    
private:
    uint8_t _state;
    InputProcessor _inputProcessor;
    std::string _userEnteredIPAddress;
    
    TitleEngineState();
    virtual ~TitleEngineState() {}
    TitleEngineState(const TitleEngineState&);
    TitleEngineState& operator=(const TitleEngineState&);
};
