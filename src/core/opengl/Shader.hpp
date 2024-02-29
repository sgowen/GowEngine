//
//  Shader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/assets/ShaderDescriptor.hpp"
#include "core/assets/FileData.hpp"
#include "core/opengl/ShaderAttribute.hpp"
#include "core/opengl/ShaderUniform.hpp"
#include "core/common/Logger.hpp"
#include "core/common/Macros.hpp"

struct Shader
{
    ShaderDescriptor _desc;
    FileData* _vertexShaderFileData;
    FileData* _fragmentShaderFileData;
    uint32_t _glHandle;
    std::vector<ShaderUniform> _uniforms;
    std::vector<ShaderAttribute> _attributes;
    
    Shader(ShaderDescriptor desc) :
    _desc(desc),
    _vertexShaderFileData(nullptr),
    _fragmentShaderFileData(nullptr),
    _glHandle(0)
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
        
        if (ret == nullptr)
        {
            // TODO, this is a hack for u_lightPositions and u_lightColors
            // sometimes we need a [0] at the end
            std::string nameWithSuffix = name + "[0]";
            for (auto& su : _uniforms)
            {
                if (su._name == nameWithSuffix)
                {
                    ret = &su;
                    break;
                }
            }
        }
        
        if (ret == nullptr)
        {
            LOG("shader uniform not found: %s", name.c_str());
        }
        
        assert(ret != nullptr);
        
        return *ret;
    }
};
