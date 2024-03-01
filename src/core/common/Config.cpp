//
//  Config.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/5/18.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

const Config Config::EMPTY = Config();

void Config::reset()
{
    _keyValues.clear();
}

bool Config::hasValues()
{
    return _keyValues.size() > 0;
}

bool Config::hasValue(std::string key)
{
    std::string* val = value(key);
    return val != nullptr;
}

bool Config::getBool(std::string key)
{
    std::string* val = value(key);
    assert(val != nullptr);
    return StringUtil::stringToBool(*val);
}

bool Config::getBool(std::string key, bool defaultValue)
{
    std::string* val = value(key);
    if (val != nullptr)
    {
        return StringUtil::stringToBool(*val);
    }
    return defaultValue;
}

int32_t Config::getInt(std::string key)
{
    std::string* val = value(key);
    assert(val != nullptr);
    return StringUtil::stringToNumber<int32_t>(*val);
}

int32_t Config::getInt(std::string key, int32_t defaultValue)
{
    std::string* val = value(key);
    if (val != nullptr)
    {
        return StringUtil::stringToNumber<int32_t>(*val);
    }
    return defaultValue;
}

uint32_t Config::getUInt(std::string key)
{
    std::string* val = value(key);
    assert(val != nullptr);
    return StringUtil::stringToNumber<uint32_t>(*val);
}

uint32_t Config::getUInt(std::string key, uint32_t defaultValue)
{    
    std::string* val = value(key);
    if (val != nullptr)
    {
        return StringUtil::stringToNumber<uint32_t>(*val);
    }
    return defaultValue;
}

uint64_t Config::getUInt64(std::string key)
{
    std::string* val = value(key);
    assert(val != nullptr);
    return StringUtil::stringToNumber<uint64_t>(*val);
}

float Config::getFloat(std::string key)
{
    std::string* val = value(key);
    assert(val != nullptr);
    return StringUtil::stringToNumber<float>(*val);
}

float Config::getFloat(std::string key, float defaultValue)
{
    std::string* val = value(key);
    if (val != nullptr)
    {
        return StringUtil::stringToNumber<float>(*val);
    }
    return defaultValue;
}

double Config::getDouble(std::string key)
{
    std::string* val = value(key);
    assert(val != nullptr);
    return StringUtil::stringToNumber<double>(*val);
}

double Config::getDouble(std::string key, double defaultValue)
{
    std::string* val = value(key);
    if (val != nullptr)
    {
        return StringUtil::stringToNumber<double>(*val);
    }
    return defaultValue;
}

std::string Config::getString(std::string key)
{
    std::string* val = value(key);
    assert(val != nullptr);
    
    std::string ret = *val;
    return ret;
}

std::string Config::getString(std::string key, std::string defaultValue)
{
    std::string* val = value(key);
    if (val != nullptr)
    {
        return *val;
    }
    return defaultValue;
}

void Config::putString(std::string key, std::string value)
{
    _keyValues.emplace(key, value);
}

void Config::putBool(std::string key, bool value)
{
    putString(key, StringUtil::boolToString(value));
}

void Config::putUInt64(std::string key, uint64_t value)
{
    putString(key, StringUtil::toString(value));
}

std::string* Config::value(std::string key)
{
    auto q = _keyValues.find(key);

    if (q != _keyValues.end())
    {
        std::string& ret = q->second;
        return &ret;
    }

    return nullptr;
}
