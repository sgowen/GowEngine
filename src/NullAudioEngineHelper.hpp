//
//  NullAudioEngineHelper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "AudioEngineHelper.hpp"

class NullAudioEngineHelper : public AudioEngineHelper
{
    friend class AudioEngineHelperFactory;
    
public:
    virtual void update() {}
    virtual void pause() {}
    virtual void resume() {}
    virtual SoundWrapper* loadSound(uint16_t soundID, const char *path, int numInstances = 1);
    virtual SoundWrapper* loadMusic(const char* path);

private:
    NullAudioEngineHelper() : AudioEngineHelper() {}
    virtual ~NullAudioEngineHelper() {}
    NullAudioEngineHelper(const NullAudioEngineHelper&);
    NullAudioEngineHelper& operator=(const NullAudioEngineHelper&);
};
