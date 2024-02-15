//
//  ConfigLoader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/27/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

class Config;

class ConfigLoader
{
public:
    static void initWithJSONFile(Config& config, std::string filePath);
    static void initWithJSON(Config& config, const char* json);
    
private:
    ConfigLoader();
    ~ConfigLoader();
    ConfigLoader(const ConfigLoader&);
    ConfigLoader& operator=(const ConfigLoader&);
};
