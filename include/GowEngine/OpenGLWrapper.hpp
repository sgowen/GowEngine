//
//  OpenGLWrapper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildMacros.hpp>

#if IS_IOS
    #include <OpenGLES/ES3/gl.h>
#elif IS_MACOS
    #include <OpenGL/OpenGL.h>
    #include <OpenGL/gl.h>
#elif IS_ANDROID
    #ifdef GL3
        #include <GLES3/gl3.h>
    #elif GL3_2
        #include <GLES3/gl32.h>
    #else
        #include <GLES2/gl2.h>
        #include <GLES2/gl2ext.h>
    #endif
#elif IS_LINUX
    #include <GL/glew.h>
#elif IS_WINDOWS
    #include <glad/gl.h>
#endif
