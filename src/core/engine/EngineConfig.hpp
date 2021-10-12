//
//  EngineConfig.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>

class Config;

class EngineConfig
{
public:
    EngineConfig(std::string configFilePath, void* data1 = nullptr, void* data2 = nullptr);
    virtual ~EngineConfig();
    
    std::string getWindowTitle();
    float getFrameRate();
    
protected:
    Config* _config;
};
