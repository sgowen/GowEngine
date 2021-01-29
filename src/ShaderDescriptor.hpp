//
//  ShaderDescriptor.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "ShaderUniform.hpp"
#include "ShaderAttribute.hpp"

#include <string>
#include <vector>

class ShaderDescriptor
{
public:
    const std::string _name;
    const std::string _vertexShaderFilePath;
    const std::string _fragmentShaderFilePath;
    
    ShaderDescriptor(std::string name, std::string vertexShaderFilePath, std::string fragmentShaderFilePath);
    
    std::vector<ShaderUniform>& getUniforms();
    std::vector<ShaderAttribute>& getAttributes();
    ShaderUniform& uniform(std::string name);
    
private:
    std::vector<ShaderUniform> _uniforms;
    std::vector<ShaderAttribute> _attributes;
};
