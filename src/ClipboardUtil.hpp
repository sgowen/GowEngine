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
    
    void init(void* data)
    {
#if IS_DESKTOP
        initWithWindow(static_cast<GLFWwindow*>(data));
#endif
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
    
private:
#if IS_DESKTOP
    GLFWwindow* _window;
    
    void initWithWindow(GLFWwindow* window)
    {
        assert(window != NULL);

        _window = window;
    }
#endif
    
    ClipboardUtil() {}
    ~ClipboardUtil() {}
    ClipboardUtil(const ClipboardUtil&);
    ClipboardUtil& operator=(const ClipboardUtil&);
};
