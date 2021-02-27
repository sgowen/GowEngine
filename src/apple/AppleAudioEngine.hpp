//
//  AppleAudioEngine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "AudioEngine.hpp"

class AppleAudioEngine : public AudioEngine
{
    friend class AudioEngineFactory;
    
public:
    virtual SoundWrapper* loadSound(const char *filePath, uint8_t numInstances = 1);
    virtual SoundWrapper* loadMusic(const char* filePath);
    virtual void onPause();
    virtual void onResume();

private:
    static AudioEngine& getInstance()
    {
        static AppleAudioEngine ret = AppleAudioEngine();
        return ret;
    }
    
    AppleAudioEngine();
    virtual ~AppleAudioEngine();
    AppleAudioEngine(const AppleAudioEngine&);
    AppleAudioEngine& operator=(const AppleAudioEngine&);
};
