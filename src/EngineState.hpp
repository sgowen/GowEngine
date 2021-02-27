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
    virtual void onEnter(Engine* e) = 0;
    virtual void onExit(Engine* e) = 0;
    virtual void onUpdate(Engine* e) = 0;
    
    virtual void enter(Engine* e);
    virtual void execute(Engine* e);
    virtual void exit(Engine* e);
    
protected:
    Assets _assets;
    Renderer _renderer;
    std::string _assetsFilePath;
    
    EngineState(std::string assetsFilePath, std::string rendererFilePath, RenderFunc rf);
    virtual ~EngineState() {}
    EngineState(const EngineState&);
    EngineState& operator=(const EngineState&);
    
private:
    void createDeviceDependentResources(Engine* e);
    void onWindowSizeChanged(Engine* e);
    void destroyDeviceDependentResources(Engine* e);
    void pause(Engine* e);
    void resume(Engine* e);
    void update(Engine* e);
    void render(Engine* e);
};
