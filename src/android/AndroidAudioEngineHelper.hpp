//
//  AndroidAudioEngineHelper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "AudioEngineHelper.hpp"

class SimpleMultiPlayer;

class AndroidAudioEngineHelper : public AudioEngineHelper
{
    friend class AudioEngineHelperFactory;
    
public:
    virtual SoundWrapper* loadSound(const char *filePath, uint8_t numInstances = 1);
    virtual SoundWrapper* loadMusic(const char* filePath);
    
private:
    static AudioEngineHelper* getInstance()
    {
        static AndroidAudioEngineHelper ret = AndroidAudioEngineHelper();
        return &ret;
    }
    
    SimpleMultiPlayer* _simpleMultiPlayer;
    
    AndroidAudioEngineHelper();
    ~AndroidAudioEngineHelper();
    AndroidAudioEngineHelper(const AndroidAudioEngineHelper&);
    AndroidAudioEngineHelper& operator=(const AndroidAudioEngineHelper&);
};
