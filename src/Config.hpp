//
//  Config.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/5/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <map>
#include <string>

class Config
{
    friend class ConfigLoader;
    
public:
    static const Config EMPTY;
    
    void initWithKeyValues(std::map<std::string, std::string> keyValues);
    bool hasValues();
    bool hasValue(std::string key);
    bool getBool(std::string key);
    int32_t getInt(std::string key);
    uint32_t getUInt(std::string key);
    float getFloat(std::string key);
    double getDouble(std::string key);
    std::string getString(std::string key);
    void putString(std::string key, std::string value);
    
private:
    std::map<std::string, std::string> _keyValues;
    
    std::string* value(std::string key);
};
