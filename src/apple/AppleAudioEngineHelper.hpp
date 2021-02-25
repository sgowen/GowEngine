//
//  AppleAudioEngineHelper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "AudioEngineHelper.hpp"

class AppleAudioEngineHelper : public AudioEngineHelper
{
    friend class AudioEngineHelperFactory;
    
public:
    virtual SoundWrapper* loadSound(const char *filePath, uint8_t numInstances = 1);
    virtual SoundWrapper* loadMusic(const char* filePath);
    virtual void pause();
    virtual void resume();

private:
    static AudioEngineHelper& getInstance()
    {
        static AppleAudioEngineHelper ret = AppleAudioEngineHelper();
        return ret;
    }
    
    AppleAudioEngineHelper();
    virtual ~AppleAudioEngineHelper();
    AppleAudioEngineHelper(const AppleAudioEngineHelper&);
    AppleAudioEngineHelper& operator=(const AppleAudioEngineHelper&);
};
