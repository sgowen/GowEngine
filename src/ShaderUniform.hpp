//
//  ShaderUniform.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>

struct ShaderUniform
{
    std::string _name;
    std::string _type;
    uint32_t _location;
    
    ShaderUniform(std::string name, std::string type) :
    _name(name),
    _type(type),
    _location(0)
    {
        // Empty
    }
};
