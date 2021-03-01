//
//  GlfwMain.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/16/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "PlatformMacros.hpp"
#if IS_DESKTOP

class EngineController;

class GlfwMain
{
public:
    static int exec(EngineController& engineController, const char* windowTitle);

private:
    GlfwMain();
    ~GlfwMain();
    GlfwMain(const GlfwMain&);
    GlfwMain& operator=(const GlfwMain&);
};

#endif /* IS_DESKTOP */
