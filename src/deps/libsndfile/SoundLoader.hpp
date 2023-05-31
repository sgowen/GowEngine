//
//  SoundLoader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/30/23.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <AL/al.h>

class SoundLoader
{
public:
    /* LoadBuffer loads the named audio file into an OpenAL buffer object, and
     * returns the new buffer ID.
     */
    static ALuint loadSound(const char *filePath);
};
