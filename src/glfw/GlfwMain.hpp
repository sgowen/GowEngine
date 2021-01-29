//
//  GlfwMain.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/16/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

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
