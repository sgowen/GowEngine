//
//  Script.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/4/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/assets/ScriptDescriptor.hpp"
#include "core/assets/FileData.hpp"

struct Script
{
    ScriptDescriptor _desc;
    FileData* _scriptFileData;
    
    Script(ScriptDescriptor desc) :
    _desc(desc),
    _scriptFileData(nullptr)
    {
        // Empty
    }
};
