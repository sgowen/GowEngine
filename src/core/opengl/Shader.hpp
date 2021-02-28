//
//  Shader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "ShaderDescriptor.hpp"

struct Shader
{
    ShaderDescriptor _desc;
    uint32_t _program;
    
    Shader(ShaderDescriptor desc) :
    _desc(desc),
    _program(0)
    {
        // Empty
    }
};
