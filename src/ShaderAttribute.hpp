//
//  ShaderAttribute.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>
#include <assert.h>

inline uint32_t sizeForType(std::string type)
{
    uint32_t ret = 0;

    if (type == "vec2")
    {
        ret = 2;
    }
    else if (type == "vec4" || type == "ivec4")
    {
        ret = 4;
    }

    assert(ret > 0);

    return ret;
}

struct ShaderAttribute
{
    std::string _name;
    std::string _type;
    uint32_t _size;
    size_t _offset;
    uint32_t _stride;
    uint32_t _location;

    ShaderAttribute(std::string name, std::string type, int count = 1) :
    _name(name),
    _type(type),
    _size(sizeForType(type) * count),
    _offset(0),
    _stride(0),
    _location(0)
    {
        // Empty
    }
};
