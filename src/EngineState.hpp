//
//  EngineState.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "StateMachine.hpp"

#include "Assets.hpp"
#include "Renderer.hpp"

#include <string>

class Engine;

class EngineState : public State<Engine>
{
public:
    virtual void enter(Engine* e);
    virtual void execute(Engine* e);
    virtual void exit(Engine* e);
    
    virtual void createDeviceDependentResources(Engine* e);
    virtual void onWindowSizeChanged(Engine* e);
    virtual void releaseDeviceDependentResources(Engine* e);
    virtual void pause(Engine* e);
    virtual void resume(Engine* e);
    virtual void update(Engine* e);
    virtual void render(Engine* e);
    
protected:
    Assets _assets;
    Renderer _renderer;
    std::string _assetsFilePath;
    
    EngineState(std::string assetsFilePath, std::string rendererFilePath);
    virtual ~EngineState() {}
    EngineState(const EngineState&);
    EngineState& operator=(const EngineState&);
};
