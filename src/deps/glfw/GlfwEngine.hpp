//
//  GlfwEngine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/16/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildConstants.hpp>
#if IS_DESKTOP

class Engine;

class GlfwEngine
{
public:
    static void exec(Engine* engine, const char* windowTitle);

private:
    GlfwEngine();
    ~GlfwEngine();
    GlfwEngine(const GlfwEngine&);
    GlfwEngine& operator=(const GlfwEngine&);
};

#endif /* IS_DESKTOP */
