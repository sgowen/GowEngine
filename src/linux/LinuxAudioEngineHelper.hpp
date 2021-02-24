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
    virtual SoundWrapper* loadSound(const char *filePath, uint8_t numInstances = 1);
    virtual SoundWrapper* loadMusic(const char* filePath);

private:
    static AudioEngineHelper* getInstance()
    {
        static LinuxAudioEngineHelper ret = LinuxAudioEngineHelper();
        return &ret;
    }
    
    LinuxAudioEngineHelper();
    ~LinuxAudioEngineHelper();
    LinuxAudioEngineHelper(const LinuxAudioEngineHelper&);
    LinuxAudioEngineHelper& operator=(const LinuxAudioEngineHelper&);
};
