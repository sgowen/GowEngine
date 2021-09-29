//
//  EngineConfig.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/entity/EntityManager.hpp"

#include <map>
#include <string>

class Config;

class EngineConfig
{
public:
    EngineConfig(std::string configFilePath, void* data1 = nullptr, void* data2 = nullptr);
    ~EngineConfig();
    
    std::string getWindowTitle();
    double getFrameRate();
    
private:
    Config* _config;
    
    void registerControllers(std::string entityManagerFilePath, std::map<std::string, EntityControllerCreationFunc>& config);
};
