//
//  RapidJSONUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/26/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>
#include <stdint.h>

#include <rapidjson/document.h>

class RapidJSONUtil
{
public:
    static std::string getString(const rapidjson::Value& v, const char* name, std::string defaultValue = "")
    {
        return v.HasMember(name) ? v[name].GetString() : defaultValue;
    }
    
    static float getFloat(const rapidjson::Value& v, const char* name, float defaultValue = 0)
    {
        return v.HasMember(name) ? v[name].GetFloat() : defaultValue;
    }
    
    static int getInt(const rapidjson::Value& v, const char* name, int defaultValue = 0)
    {
        return v.HasMember(name) ? v[name].GetInt() : defaultValue;
    }
    
    static uint32_t getUInt(const rapidjson::Value& v, const char* name, uint32_t defaultValue = 0)
    {
        return v.HasMember(name) ? v[name].GetUint() : defaultValue;
    }
    
    static bool getBool(const rapidjson::Value& v, const char* name, bool defaultValue = false)
    {
        return v.HasMember(name) ? v[name].GetBool() : defaultValue;
    }
    
private:
    RapidJSONUtil();
    ~RapidJSONUtil();
    RapidJSONUtil(const RapidJSONUtil&);
    RapidJSONUtil& operator=(const RapidJSONUtil&);
};
