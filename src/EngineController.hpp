//
//  EngineController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "StateMachine.hpp"
#include "PlatformMacros.hpp"
#include "EntityManager.hpp"

#include <map>
#include <string>

class Engine;

class EngineController
{
public:
    virtual State<Engine>& getInitialState() = 0;
    virtual double getFrameRate();
    
protected:
    void registerControllers(std::map<std::string, EntityControllerCreationFunc>& config, std::string entityManagerFilePath);
    void configureForNetwork(std::map<std::string, EntityNetworkControllerCreationFunc>& config, std::string entityLayoutManagerFilePath);
    void registerPhysicsControllers(std::map<std::string, EntityPhysicsControllerCreationFunc>& config);
    void registerRenderControllers(std::map<std::string, EntityRenderControllerCreationFunc>& config);
    
#if IS_ANDROID
    EngineController(void* data1, void* data2);
#else
    EngineController(void* data1 = NULL, void* data2 = NULL);
#endif
    virtual ~EngineController();
};
