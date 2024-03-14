//
//  Script.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/4/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/assets/FileDescriptor.hpp"
#include "core/assets/FileData.hpp"

struct Script
{
    FileDescriptor _desc;
    FileData* _fileData;
    
    Script(FileDescriptor desc) :
    _desc(desc),
    _fileData(nullptr)
    {
        // Empty
    }
};
