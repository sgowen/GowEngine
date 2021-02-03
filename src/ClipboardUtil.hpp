//
//  ClipboardUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/02/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "PlatformMacros.hpp"

#if IS_DESKTOP
    #define GLFW_INCLUDE_NONE
    #include <GLFW/glfw3.h>
#else
    // TODO
#endif

#define CLIPBOARD_UTIL ClipboardUtil::getInstance()

class ClipboardUtil
{
public:
    static ClipboardUtil& getInstance()
    {
        static ClipboardUtil ret = ClipboardUtil();
        return ret;
    }
    
    const char* getClipboardString()
    {
#if IS_DESKTOP
        return glfwGetClipboardString(_window);
#else
        // TODO
        return "";
#endif
    }
    
    void init(void* window)
    {
        assert(window != NULL);

#if IS_DESKTOP
        _window = static_cast<GLFWwindow*>(window);
#endif
    }
    
private:
#if IS_DESKTOP
    GLFWwindow* _window;
#endif
    
    ClipboardUtil() {}
    ~ClipboardUtil() {}
    ClipboardUtil(const ClipboardUtil&);
    ClipboardUtil& operator=(const ClipboardUtil&);
};
