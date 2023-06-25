//
//  Shader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/assets/ShaderDescriptor.hpp"
#include "core/assets/FileData.hpp"

struct Shader
{
    ShaderDescriptor _desc;
    FileData* _vertexShaderFileData;
    FileData* _fragmentShaderFileData;
    uint32_t _glHandle;
    
    Shader(ShaderDescriptor desc) :
    _desc(desc),
    _vertexShaderFileData(nullptr),
    _fragmentShaderFileData(nullptr),
    _glHandle(0)
    {
        // Empty
    }
};
