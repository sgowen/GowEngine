//
//  Sound.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/24/23.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/assets/SoundDescriptor.hpp"

struct Sound
{
    SoundDescriptor _desc;
    uint32_t _alHandle;
    
    Sound(SoundDescriptor desc) :
    _desc(desc),
    _alHandle(0)
    {
        // Empty
    }
};
