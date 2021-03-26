//
//  ClipboardUtil.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/26/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_DESKTOP
    #include <GLFW/glfw3.h>
#endif

void ClipboardUtil::init(void* data)
{
#if IS_DESKTOP
    initWithWindow(static_cast<GLFWwindow*>(data));
#else
    // TODO, add support for mobile
    UNUSED(data);
#endif
}

const char* ClipboardUtil::getClipboardString()
{
#if IS_DESKTOP
    return glfwGetClipboardString(_window);
#else
    // TODO, add support for mobile
    return "";
#endif
}

#if IS_DESKTOP
void ClipboardUtil::initWithWindow(GLFWwindow* window)
{
    assert(window != NULL);

    _window = window;
}
#endif
