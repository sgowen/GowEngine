//
//  Config.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/5/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "Config.hpp"

#include "AssetHandlerFactory.hpp"
#include "AssetHandler.hpp"
#include "FileData.hpp"
#include "StringUtil.hpp"

#include <rapidjson/document.h>

#include <assert.h>

const Config Config::EMPTY = Config();

void Config::initWithJSONFile(const char* filePath)
{
    AssetHandler* ah = AssetHandlerFactory::create();
    FileData jsonData = ah->loadAsset(filePath);
    initWithJSON((const char*)jsonData._data);
    ah->releaseAsset(jsonData);
    AssetHandlerFactory::destroy(ah);
}

void Config::initWithJSON(const char* json)
{
    _keyValues.clear();
    
    using namespace rapidjson;
    
    Document d;
    d.Parse<kParseStopWhenDoneFlag>(json);
    assert(d.IsObject());
    for (Value::ConstMemberIterator i = d.MemberBegin(); i != d.MemberEnd(); ++i)
    {
        assert(i->value.IsString());
        
        _keyValues[i->name.GetString()] = i->value.GetString();
    }
}

void Config::initWithKeyValues(std::map<std::string, std::string> keyValues)
{
    _keyValues = keyValues;
}

bool Config::hasValues()
{
    return _keyValues.size() > 0;
}

bool Config::hasValue(std::string key)
{
    std::string* val = findValue(key);
    return val != NULL;
}

bool Config::getBool(std::string key)
{
    std::string* val = findValue(key);
    assert(val != NULL);
    bool ret = StringUtil::stringToBool(*val);
    return ret;
}

int32_t Config::getInt(std::string key)
{
    std::string* val = findValue(key);
    assert(val != NULL);
    int32_t ret = StringUtil::stringToNumber<int32_t>(*val);
    return ret;
}

uint32_t Config::getUInt(std::string key)
{
    std::string* val = findValue(key);
    assert(val != NULL);
    uint32_t ret = StringUtil::stringToNumber<uint32_t>(*val);
    return ret;
}

float Config::getFloat(std::string key)
{
    std::string* val = findValue(key);
    assert(val != NULL);
    float ret = StringUtil::stringToNumber<float>(*val);
    return ret;
}

double Config::getDouble(std::string key)
{
    std::string* val = findValue(key);
    assert(val != NULL);
    double ret = StringUtil::stringToNumber<double>(*val);
    return ret;
}

std::string Config::getString(std::string key)
{
    std::string* val = findValue(key);
    assert(val != NULL);
    
    std::string ret = *val;
    return ret;
}

std::map<std::string, std::string>& Config::getMap()
{
    return _keyValues;
}

std::string* Config::findValue(std::string key)
{
    auto q = _keyValues.find(key);

    if (q != _keyValues.end())
    {
        std::string& ret = q->second;
        return &ret;
    }

    return NULL;
}
