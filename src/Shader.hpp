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
    ShaderDescriptor _descriptor;
    uint32_t _program;
    
    Shader(ShaderDescriptor sd) :
    _descriptor(sd),
    _program(0)
    {
        // Empty
    }
};
