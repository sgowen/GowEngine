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

namespace sol { class state; }

struct Script
{
    FileDescriptor _desc;
    FileData* _fileData;
    sol::state* _lua;
    
    Script(FileDescriptor desc) :
    _desc(desc),
    _fileData(nullptr),
    _lua(nullptr)
    {
        // Empty
    }
};
