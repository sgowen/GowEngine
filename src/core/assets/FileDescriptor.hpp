//
//  FileDescriptor.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/4/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>

struct FileDescriptor
{
    const std::string _name;
    const std::string _filePath;
    
    FileDescriptor(std::string name, std::string filePath) :
    _name(name),
    _filePath(filePath)
    {
        // Empty
    }
};
