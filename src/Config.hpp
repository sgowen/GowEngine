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
public:
    Config();
    ~Config();
    
    void initWithJSONFile(const char* filePath);
    void initWithJSON(const char* json);
    void clear();
    bool getBool(std::string key);
    int getInt(std::string key);
    float getFloat(std::string key);
    double getDouble(std::string key);
    std::string getString(std::string key);
    
private:
    std::map<std::string, std::string> _keyValues;
    
    std::string findValue(std::string key);
};
