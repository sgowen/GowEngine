//
//  AudioEngineHelper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

class SoundWrapper;

class AudioEngineHelper
{
    friend class AudioEngineHelperFactory;
    
public:
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual SoundWrapper* loadSound(uint16_t soundID, const char *path, int numInstances = 1) = 0;
    virtual SoundWrapper* loadMusic(const char* path) = 0;
    
protected:
    AudioEngineHelper() {}
    virtual ~AudioEngineHelper() {}
};
