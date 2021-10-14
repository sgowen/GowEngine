//
//  EngineConfig.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>

#include "core/common/Config.hpp"

class EngineState;

class EngineConfig
{
public:
    EngineConfig(std::string configFilePath, EngineState& initialEngineState);
    
    EngineState& initialEngineState();
    
private:
    Config _config;
    EngineState& _initialEngineState;
};
