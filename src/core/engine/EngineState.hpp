//
//  EngineState.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/StateMachine.hpp"
#include "core/assets/Assets.hpp"

#include <string>

class Engine;
class Config;
class Renderer;

class EngineState : public State<Engine>
{
public:
    virtual void enter(Engine* e) override;
    virtual void execute(Engine* e) override;
    virtual void exit(Engine* e) override;
    
    void createDeviceDependentResources(Engine* e);
    void onWindowSizeChanged(Engine* e);
    void destroyDeviceDependentResources(Engine* e);
    void pause(Engine* e);
    void resume(Engine* e);
    void render(Engine* e);
    
protected:
    virtual void onEnter(Engine* e) = 0;
    virtual void onAssetsLoaded(Engine* e) = 0;
    virtual void onExit(Engine* e) = 0;
    virtual void onUpdate(Engine* e) = 0;
    virtual void onRender(Renderer& r) = 0;
    
    Config _config;
    
    EngineState(std::string configFilePath, std::string filePathAssets);
    virtual ~EngineState() {}
    EngineState(const EngineState&);
    EngineState& operator=(const EngineState&);
    
private:
    std::string _configFilePath;
    std::string _filePathAssets;
    Assets _assets;
};
