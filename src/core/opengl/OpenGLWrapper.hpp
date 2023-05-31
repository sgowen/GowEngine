//
//  OpenGLWrapper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildConstants.hpp>

#if IS_ANDROID
    #include <GLES2/gl2.h>
#elif IS_APPLE
    #if IS_IOS
        #include <OpenGLES/ES2/gl.h>
    #elif IS_MACOS
        #include <OpenGL/gl.h>
    #endif
#elif IS_LINUX || IS_WINDOWS
    #include <glad/gl.h>
#endif
