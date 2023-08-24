//
//  Logger.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 8/18/23.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <stdarg.h>

#if IS_ANDROID
    #include <android/log.h>
#elif IS_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <debugapi.h>
#endif

// OUT_Z_ARRAY indicates an output array that will be null-terminated.
#if _MSC_VER >= 1600
    // Include the annotation header file.
    #include <sal.h>
    #if _MSC_VER >= 1700
        // VS 2012+
        #define OUT_Z_ARRAY _Post_z_
    #else
        // VS 2010
        #define OUT_Z_ARRAY _Deref_post_z_
    #endif
#else
    // gcc, clang, old versions of VS
    #define OUT_Z_ARRAY
#endif

#if IS_WINDOWS
// OutputDebugStringA is included with the Windows API
#else
void OutputDebugStringA(const char* value)
{
#if IS_ANDROID
    __android_log_print(ANDROID_LOG_DEBUG, "GowEngine", "%s", value);
#else
    printf("%s", value);
#endif
}
#endif

void Logger::initWithFile(const char* fileName)
{
    _fileStream.open(fileName, std::ios::app);
}

void Logger::closeFileStream()
{
    _fileStream.close();
}

void Logger::log(const char* format, ...)
{
    static const int size = 4096;
    char temp[size];
    
    va_list args;
    va_start (args, format);
    
#if IS_WINDOWS
    _vsnprintf_s(temp, size, size, format, args);
#elif IS_ANDROID
    __android_log_vprint(ANDROID_LOG_DEBUG, "GowEngine", format, args);
#else
    vsnprintf(temp, size, format, args);
#endif
    
    if (ENGINE_CFG.consoleLoggingEnabled())
    {
        OutputDebugStringA(temp);
        OutputDebugStringA("\n");
    }
    
    if (ENGINE_CFG.fileLoggingEnabled() &&
        _fileStream)
    {
        _fileStream << temp;
        _fileStream << "\n";
    }
}
