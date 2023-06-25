//
//  SoundLoader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/30/23.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

class SoundLoader
{
public:
    /* LoadBuffer loads the named audio file into an OpenAL buffer object, and
     * returns the new buffer ID.
     */
    static uint32_t loadSound(const char *filePath);
    
//    void loadSound(Sound& s);
//    void unloadSound(Sound& s);
};
