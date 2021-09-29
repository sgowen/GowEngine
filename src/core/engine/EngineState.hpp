//
//  EngineState.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/StateMachine.hpp"
#include "core/assets/Assets.hpp"
#include "core/opengl/Renderer.hpp"

#include <string>

class Engine;
class Config;

class EngineState : public State<Engine>
{
public:
    virtual void onEnter(Engine* e) = 0;
    virtual void onExit(Engine* e) = 0;
    virtual void onUpdate(Engine* e) = 0;
    virtual void onRender(Renderer& r) = 0;
    
    virtual void enter(Engine* e);
    virtual void execute(Engine* e);
    virtual void exit(Engine* e);
    
protected:
    Config _config;
    
    EngineState(std::string configFilePath);
    virtual ~EngineState() {}
    EngineState(const EngineState&);
    EngineState& operator=(const EngineState&);
    
private:
    std::string _assetsFilePath;
    Assets _assets;
    Renderer _renderer;
    
    void createDeviceDependentResources(Engine* e);
    void onWindowSizeChanged(Engine* e);
    void destroyDeviceDependentResources(Engine* e);
    void pause(Engine* e);
    void resume(Engine* e);
    void update(Engine* e);
    void render(Engine* e);
};
