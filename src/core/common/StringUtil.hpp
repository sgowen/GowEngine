//
//  StringUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/9/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildMacros.hpp>

#include <string>
#include <sstream>
#include <cstdarg>
#include <stdarg.h>
#include <cassert>

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

#define LOG(...) StringUtil::log(__VA_ARGS__)
#define STRING_FORMAT(...) StringUtil::format(__VA_ARGS__)

class StringUtil
{
public:
    template<typename T>
    static std::string numberToString(const T& n)
    {
        std::ostringstream ss;
        ss << n;
        return ss.str();
    }

    template<typename T>
    static T stringToNumber(const std::string& str, T defaultValue = 0)
    {
        std::stringstream ss(str);
        T value;
        if ((ss >> value).fail())
        {
            return defaultValue;
        }

        return value;
    }
    
    static std::string boolToString(bool value)
    {
        return value ? "true" : "false";
    }
    
    static bool stringToBool(const std::string& str, bool defaultValue = false)
    {
        if (str == "true" || str == "yes" || str == "1")
        {
            return true;
        }
        else if (str == "false" || str == "no" || str == "0")
        {
            return false;
        }
        
        return defaultValue;
    }
    
    static std::string format(const char* format, ...)
    {
        static int size = 4096;
        char temp[size];
        
        va_list args;
        va_start (args, format);
        
    #if IS_WINDOWS
        _vsnprintf_s(temp, size, size, format, args);
    #else
        vsnprintf(temp, size, format, args);
    #endif
        
        return std::string(temp);
    }
    
#if IS_WINDOWS
    // OutputDebugStringA is included with the Windows API
#else
    static void OutputDebugStringA(const char* value)
    {
#if IS_ANDROID
        __android_log_print(ANDROID_LOG_DEBUG, "GowEngine", "%s", value);
#else
        printf("%s", value);
#endif
    }
#endif

    static void log(const char* format, ...)
    {
        // TODO, replace with spdlog
        static int size = 4096;
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
        
        OutputDebugStringA(temp);
        OutputDebugStringA("\n");
    }

    static std::string stringFromFourChar(uint32_t fourCC)
    {
        char chars[5];
        
        chars[4] = '\0';
        chars[3] = (char)(fourCC & 0xFF);
        chars[2] = (char)(fourCC >> 8 & 0xFF);
        chars[1] = (char)(fourCC >> 16 & 0xFF);
        chars[0] = (char)(fourCC >> 24 & 0xFF);
        
        return std::string(chars);
    }

    static uint32_t fourCharFromString(std::string& fourCC)
    {
        assert(fourCC.length() == 4);
        
        const char* chars = fourCC.c_str();
        
        return
        (uint32_t)chars[0] << 24 |
        (uint32_t)chars[1] << 16 |
        (uint32_t)chars[2] << 8  |
        (uint32_t)chars[3];
    }

private:
    StringUtil();
    ~StringUtil();
    StringUtil(const StringUtil&);
    StringUtil& operator=(const StringUtil&);
};
