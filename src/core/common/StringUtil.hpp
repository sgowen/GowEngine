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

#define LOG(...) StringUtil::log(__VA_ARGS__)

class StringUtil
{
public:
    template<typename T>
    static std::string toString(const T& n)
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
        char temp[4096];
        
        va_list args;
        va_start (args, format);
        
    #if IS_WINDOWS
        _vsnprintf_s(temp, 4096, 4096, format, args);
    #else
        vsnprintf(temp, 4096, format, args);
    #endif
        
        return std::string(temp);
    }

    static void log(const char* format, ...)
    {
        // TODO, replace with spdlog
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
