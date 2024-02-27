//
//  StringUtil.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/9/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <stdarg.h>

#if IS_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
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

std::string StringUtil::toString(uint8_t value)
{
    return std::to_string(value);
}

std::string StringUtil::toString(uint64_t value)
{
    return std::to_string(value);
}

std::string StringUtil::boolToString(bool value)
{
    return value ? "true" : "false";
}

bool StringUtil::stringToBool(const std::string& str, bool defaultValue)
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

std::string StringUtil::format(const char* format, ...)
{
    static const int size = 4096;
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

std::string StringUtil::stringFromFourChar(uint32_t fourCC)
{
    char chars[5];
    
    chars[4] = '\0';
    chars[3] = (char)(fourCC & 0xFF);
    chars[2] = (char)(fourCC >> 8 & 0xFF);
    chars[1] = (char)(fourCC >> 16 & 0xFF);
    chars[0] = (char)(fourCC >> 24 & 0xFF);
    
    return std::string(chars);
}

uint32_t StringUtil::fourCharFromString(std::string& fourCC)
{
    assert(fourCC.length() == 4);
    
    const char* chars = fourCC.c_str();
    
    return
    (uint32_t)chars[0] << 24 |
    (uint32_t)chars[1] << 16 |
    (uint32_t)chars[2] << 8  |
    (uint32_t)chars[3];
}

bool StringUtil::doesStringStartWithString(const std::string& fullString, const std::string& start)
{
    if (start.size() > fullString.size())
    {
        return false;
    }
    
    return equal(start.begin(), start.end(), fullString.begin());
}

bool StringUtil::doesStringEndWithString(const std::string& fullString, const std::string& ending)
{
    if (ending.size() > fullString.size())
    {
        return false;
    }
    
    return fullString.compare(fullString.size() - ending.size(), ending.size(), ending) == 0;
}
