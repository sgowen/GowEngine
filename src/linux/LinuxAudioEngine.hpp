//
//  LinuxAudioEngine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/21/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "AudioEngine.hpp"

class LinuxAudioEngine : public AudioEngine
{
    friend class AudioEngineFactory;
    
public:
    virtual SoundWrapper* loadSound(const char *filePath, uint8_t numInstances = 1);
    virtual SoundWrapper* loadMusic(const char* filePath);

private:
    static AudioEngine* getInstance()
    {
        static LinuxAudioEngine ret = LinuxAudioEngine();
        return &ret;
    }
    
    LinuxAudioEngine();
    ~LinuxAudioEngine();
    LinuxAudioEngine(const LinuxAudioEngine&);
    LinuxAudioEngine& operator=(const LinuxAudioEngine&);
};
