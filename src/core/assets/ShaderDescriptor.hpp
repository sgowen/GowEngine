//
//  ShaderDescriptor.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/opengl/ShaderUniform.hpp"
#include "core/opengl/ShaderAttribute.hpp"

#include <string>
#include <vector>

struct ShaderDescriptor
{
    const std::string _name;
    const std::string _vertexShaderFilePath;
    const std::string _fragmentShaderFilePath;
    std::vector<ShaderUniform> _uniforms;
    std::vector<ShaderAttribute> _attributes;
    
    ShaderDescriptor(std::string name, std::string vertexShaderFilePath, std::string fragmentShaderFilePath) :
    _name(name),
    _vertexShaderFilePath(vertexShaderFilePath),
    _fragmentShaderFilePath(fragmentShaderFilePath)
    {
        // Empty
    }
    
    ShaderUniform& uniform(std::string name)
    {
        ShaderUniform* ret = nullptr;
        
        for (auto& su : _uniforms)
        {
            if (su._name == name)
            {
                ret = &su;
                break;
            }
        }
        
        assert(ret != nullptr);
        
        return *ret;
    }
};
