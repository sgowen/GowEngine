//
//  ConfigLoader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/27/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Config.hpp"

class ConfigLoader
{
public:
    static Config initWithJSONFile(const char* filePath);
    static Config initWithJSON(const char* json);
    
private:
    ConfigLoader();
    ~ConfigLoader();
    ConfigLoader(const ConfigLoader&);
    ConfigLoader& operator=(const ConfigLoader&);
};
