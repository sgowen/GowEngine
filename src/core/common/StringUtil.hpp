//
//  StringUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/9/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>

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
    
    static std::string toString(uint8_t value);
    static std::string toString(uint64_t value);
    static std::string boolToString(bool value);
    static bool stringToBool(const std::string& str, bool defaultValue = false);
    static std::string format(const char* format, ...);
    static std::string stringFromFourChar(uint32_t fourCC);
    static uint32_t fourCharFromString(std::string& fourCC);
    static bool doesStringStartWithString(const std::string& fullString, const std::string& start);
    static bool doesStringEndWithString(const std::string& fullString, const std::string& ending);

private:
    StringUtil();
    ~StringUtil();
    StringUtil(const StringUtil&);
    StringUtil& operator=(const StringUtil&);
};
