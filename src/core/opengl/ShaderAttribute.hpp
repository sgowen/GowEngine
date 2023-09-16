//
//  ShaderAttribute.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>

struct ShaderAttribute
{
    std::string _name;
    uint32_t _type;
    uint32_t _glHandle;
    uint32_t _size;
    size_t _offset;
    uint32_t _stride;

    ShaderAttribute(std::string name,
                    uint32_t type,
                    uint32_t glHandle) :
    _name(name),
    _type(type),
    _glHandle(glHandle),
    _size(0),
    _offset(0),
    _stride(0)
    {
        // Empty
    }
};
