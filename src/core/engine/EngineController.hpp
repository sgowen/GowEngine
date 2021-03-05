//
//  EngineController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "StateMachine.hpp"
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
    void configureForNetwork(std::string entityManagerFilePath, std::string entityLayoutManagerFilePath, std::map<std::string, EntityControllerCreationFunc>& config);
    void configureForOffline(std::string entityManagerFilePath, std::string entityLayoutManagerFilePath, std::map<std::string, EntityControllerCreationFunc>& config);
    
    EngineController(void* data1 = NULL, void* data2 = NULL);
    virtual ~EngineController();
    
private:
    void registerControllers(std::string entityManagerFilePath, std::map<std::string, EntityControllerCreationFunc>& config);
};
