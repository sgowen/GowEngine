//
//  LinuxAudioEngineHelper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/21/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "AudioEngineHelper.hpp"

class LinuxAudioEngineHelper : public AudioEngineHelper
{
    friend class AudioEngineHelperFactory;
    
public:
    virtual void pause() {}
    virtual void resume() {}
    virtual SoundWrapper* loadSound(uint16_t soundID, const char *filePath, int numInstances = 1);
    virtual SoundWrapper* loadMusic(const char* filePath);

private:
    LinuxAudioEngineHelper();
    ~LinuxAudioEngineHelper();
    LinuxAudioEngineHelper(const LinuxAudioEngineHelper&);
    LinuxAudioEngineHelper& operator=(const LinuxAudioEngineHelper&);
};
