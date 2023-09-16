//
//  ConfigLoader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/27/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/Config.hpp"

class ConfigLoader
{
public:
    static Config initWithJSONFile(std::string filePath);
    static Config initWithJSON(const char* json);
    
private:
    ConfigLoader();
    ~ConfigLoader();
    ConfigLoader(const ConfigLoader&);
    ConfigLoader& operator=(const ConfigLoader&);
};
