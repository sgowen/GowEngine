//
//  GlfwEngine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/16/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildMacros.hpp>
#if IS_DESKTOP

class EngineState;

class GlfwEngine
{
public:
    static void exec(std::string configFilePath, EngineState& initialEngineState, const char* windowTitle);

private:
    GlfwEngine();
    ~GlfwEngine();
    GlfwEngine(const GlfwEngine&);
    GlfwEngine& operator=(const GlfwEngine&);
};

#endif /* IS_DESKTOP */
