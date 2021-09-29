//
//  GlfwMain.hpp
//  GowEngineMount
//
//  Created by Stephen Gowen on 11/16/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildMacros.hpp>
#if IS_DESKTOP

class EngineConfig;
class EngineState;
struct GLFWwindow;

class GlfwMain
{
public:
    static void exec(EngineConfig& engineConfig, EngineState& initialEngineState);

private:
    static void runEngine(EngineConfig& engineConfig, EngineState& initialEngineState, GLFWwindow* window);
    
    GlfwMain();
    ~GlfwMain();
    GlfwMain(const GlfwMain&);
    GlfwMain& operator=(const GlfwMain&);
};

#endif /* IS_DESKTOP */
