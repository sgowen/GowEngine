//
//  ShaderDescriptor.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>

struct ShaderDescriptor
{
    const std::string _name;
    const std::string _vertexShaderFilePath;
    const std::string _fragmentShaderFilePath;
    
    ShaderDescriptor(std::string name, std::string vertexShaderFilePath, std::string fragmentShaderFilePath) :
    _name(name),
    _vertexShaderFilePath(vertexShaderFilePath),
    _fragmentShaderFilePath(fragmentShaderFilePath)
    {
        // Empty
    }
};
