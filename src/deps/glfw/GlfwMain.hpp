//
//  GlfwMain.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/16/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildMacros.hpp>
#if IS_DESKTOP

class EngineConfig;

class GlfwMain
{
public:
    static void exec(EngineConfig& engineConfig, const char* windowTitle);

private:
    GlfwMain();
    ~GlfwMain();
    GlfwMain(const GlfwMain&);
    GlfwMain& operator=(const GlfwMain&);
};

#endif /* IS_DESKTOP */
