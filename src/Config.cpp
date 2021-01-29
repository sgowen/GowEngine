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

void Config::initWithJSONFile(const char* filePath)
{
    AssetHandler* ah = AssetHandlerFactory::create();
    FileData jsonData = ah->loadAsset(filePath);
    initWithJSON(jsonData._data);
    ah->releaseAsset(jsonData);
    AssetHandlerFactory::destroy(ah);
}

void Config::initWithJSON(const char* json)
{
    clear();
    
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

void Config::clear()
{
    _keyValues.clear();
}

bool Config::getBool(std::string key)
{
    std::string val = findValue(key);
    bool ret = StringUtil::stringToBool(val);
    return ret;
}

int Config::getInt(std::string key)
{
    std::string val = findValue(key);
    int ret = StringUtil::stringToNumber<int>(val);
    return ret;
}

float Config::getFloat(std::string key)
{
    std::string val = findValue(key);
    float ret = StringUtil::stringToNumber<float>(val);
    return ret;
}

double Config::getDouble(std::string key)
{
    std::string val = findValue(key);
    double ret = StringUtil::stringToNumber<double>(val);
    return ret;
}

std::string Config::getString(std::string key)
{
    return findValue(key);
}

std::string Config::findValue(std::string key)
{
    auto q = _keyValues.find(key);

    if (q != _keyValues.end())
    {
        return q->second;
    }

    return "";
}
