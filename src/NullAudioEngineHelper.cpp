//
//  NullAudioEngineHelper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "NullAudioEngineHelper.hpp"

#include "NullSoundWrapper.hpp"

SoundWrapper* NullAudioEngineHelper::loadSound(uint16_t soundID, const char *path, int numInstances)
{
    NullSoundWrapper* sound = new NullSoundWrapper(soundID, path, numInstances);
    
    return sound;
}

SoundWrapper* NullAudioEngineHelper::loadMusic(const char* path)
{
    return loadSound(1337, path);
}
