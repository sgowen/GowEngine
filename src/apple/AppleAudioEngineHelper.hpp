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
    virtual void pause();
    virtual void resume();
    virtual SoundWrapper* loadSound(uint16_t soundID, const char *filePath, int numInstances = 1);
    virtual SoundWrapper* loadMusic(const char* filePath);

private:
    AppleAudioEngineHelper();
    virtual ~AppleAudioEngineHelper();
    AppleAudioEngineHelper(const AppleAudioEngineHelper&);
    AppleAudioEngineHelper& operator=(const AppleAudioEngineHelper&);
};
