//
//  Config.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/5/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "Config.hpp"

#include "StringUtil.hpp"

#include <assert.h>

const Config Config::EMPTY = Config();

bool Config::hasValues()
{
    return _keyValues.size() > 0;
}

bool Config::hasValue(std::string key)
{
    std::string* val = value(key);
    return val != NULL;
}

bool Config::getBool(std::string key)
{
    std::string* val = value(key);
    assert(val != NULL);
    bool ret = StringUtil::stringToBool(*val);
    return ret;
}

int32_t Config::getInt(std::string key)
{
    std::string* val = value(key);
    assert(val != NULL);
    int32_t ret = StringUtil::stringToNumber<int32_t>(*val);
    return ret;
}

uint32_t Config::getUInt(std::string key)
{
    std::string* val = value(key);
    assert(val != NULL);
    uint32_t ret = StringUtil::stringToNumber<uint32_t>(*val);
    return ret;
}

uint32_t Config::getUInt(std::string key, uint32_t defaultValue)
{    
    std::string* val = value(key);
    if (val != NULL)
    {
        return StringUtil::stringToNumber<uint32_t>(*val);
    }
        
    return defaultValue;
}

float Config::getFloat(std::string key)
{
    std::string* val = value(key);
    assert(val != NULL);
    float ret = StringUtil::stringToNumber<float>(*val);
    return ret;
}

double Config::getDouble(std::string key)
{
    std::string* val = value(key);
    assert(val != NULL);
    double ret = StringUtil::stringToNumber<double>(*val);
    return ret;
}

std::string Config::getString(std::string key)
{
    std::string* val = value(key);
    assert(val != NULL);
    
    std::string ret = *val;
    return ret;
}

void Config::putString(std::string key, std::string value)
{
    _keyValues.emplace(key, value);
}

std::string* Config::value(std::string key)
{
    auto q = _keyValues.find(key);

    if (q != _keyValues.end())
    {
        std::string& ret = q->second;
        return &ret;
    }

    return NULL;
}
