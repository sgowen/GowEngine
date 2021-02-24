//
//  WindowsAudioEngineHelper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "AudioEngineHelper.hpp"

#include "Audio.h"

#include <memory>

class WindowsAudioEngineHelper : public AudioEngineHelper
{
    friend class AudioEngineHelperFactory;
    
public:
    virtual SoundWrapper* loadSound(const char *filePath, uint8_t numInstances = 1);
    virtual SoundWrapper* loadMusic(const char* filePath);
    virtual void pause();
    virtual void resume();

private:
    static AudioEngineHelper* getInstance()
    {
        static WindowsAudioEngineHelper ret = WindowsAudioEngineHelper();
        return &ret;
    }
    
    std::unique_ptr<DirectX::AudioEngine> _audioEngine;
    
    WindowsAudioEngineHelper();
    virtual ~WindowsAudioEngineHelper();
    WindowsAudioEngineHelper(const WindowsAudioEngineHelper&);
    WindowsAudioEngineHelper& operator=(const WindowsAudioEngineHelper&);
};
