//
//  ClipboardUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/02/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildMacros.hpp>

#define CLIPBOARD_UTIL ClipboardUtil::getInstance()

#if IS_DESKTOP
    // fixme
    struct GLFWwindow;
#endif

class ClipboardUtil
{
public:
    static ClipboardUtil& getInstance()
    {
        static ClipboardUtil ret = ClipboardUtil();
        return ret;
    }
    
    void init(void* data);
    const char* getClipboardString();
    
private:
#if IS_DESKTOP
    // fixme
    GLFWwindow* _window;
    
    void initWithWindow(GLFWwindow* window);
#endif
    
    ClipboardUtil() {}
    ~ClipboardUtil() {}
    ClipboardUtil(const ClipboardUtil&);
    ClipboardUtil& operator=(const ClipboardUtil&);
};
