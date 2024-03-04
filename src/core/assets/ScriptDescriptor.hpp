//
//  ScriptDescriptor.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/4/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>

struct ScriptDescriptor
{
    const std::string _name;
    const std::string _scriptFilePath;
    
    ScriptDescriptor(std::string name, std::string scriptFilePath) :
    _name(name),
    _scriptFilePath(scriptFilePath)
    {
        // Empty
    }
};
