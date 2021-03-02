//
//  Shader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "ShaderDescriptor.hpp"
#include "FileData.hpp"

struct Shader
{
    ShaderDescriptor _desc;
    FileData* _vertexShaderFileData;
    FileData* _fragmentShaderFileData;
    uint32_t _program;
    
    Shader(ShaderDescriptor desc) :
    _desc(desc),
    _vertexShaderFileData(NULL),
    _fragmentShaderFileData(NULL),
    _program(0)
    {
        // Empty
    }
};
