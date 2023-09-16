//
//  ShaderUniform.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>

struct ShaderUniform
{
    std::string _name;
    uint32_t _type;
    uint32_t _glHandle;
    
    ShaderUniform(std::string name, uint32_t type, uint32_t glHandle) :
    _name(name),
    _type(type),
    _glHandle(glHandle)
    {
        // Empty
    }
};
