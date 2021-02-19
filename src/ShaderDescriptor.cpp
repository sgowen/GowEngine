//
//  ShaderDescriptor.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "ShaderDescriptor.hpp"

ShaderDescriptor::ShaderDescriptor(std::string name, std::string vertexShaderFilePath, std::string fragmentShaderFilePath) :
_name(name),
_vertexShaderFilePath(vertexShaderFilePath),
_fragmentShaderFilePath(fragmentShaderFilePath)
{
    // Empty
}

std::vector<ShaderUniform>& ShaderDescriptor::getUniforms()
{
    return _uniforms;
}

std::vector<ShaderAttribute>& ShaderDescriptor::getAttributes()
{
    return _attributes;
}

ShaderUniform& ShaderDescriptor::uniform(std::string name)
{
    ShaderUniform* ret = NULL;
    
    for (auto& su : _uniforms)
    {
        if (su._name == name)
        {
            ret = &su;
            break;
        }
    }
    
    assert(ret != NULL);
    
    return *ret;
}
