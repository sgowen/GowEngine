//
//  Config.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/5/18.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <map>
#include <string>

class Config
{    
public:
    static const Config EMPTY;
    
    bool hasValues();
    bool hasValue(std::string key);
    bool getBool(std::string key);
    bool getBool(std::string key, bool defaultValue);
    int32_t getInt(std::string key);
    int32_t getInt(std::string key, int32_t defaultValue);
    uint32_t getUInt(std::string key);
    uint32_t getUInt(std::string key, uint32_t defaultValue);
    float getFloat(std::string key);
    float getFloat(std::string key, float defaultValue);
    double getDouble(std::string key);
    double getDouble(std::string key, double defaultValue);
    std::string getString(std::string key);
    std::string getString(std::string key, std::string defaultValue);
    void putString(std::string key, std::string value);
    void putBool(std::string key, bool value);
    
private:
    std::map<std::string, std::string> _keyValues;
    
    std::string* value(std::string key);
};
