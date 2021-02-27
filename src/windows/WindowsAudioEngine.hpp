//
//  WindowsAudioEngine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "AudioEngine.hpp"

#include "Audio.h"

#include <memory>

class WindowsAudioEngine : public AudioEngine
{
    friend class AudioEngineFactory;
    
public:
    virtual SoundWrapper* loadSound(const char *filePath, uint8_t numInstances = 1);
    virtual SoundWrapper* loadMusic(const char* filePath);
    virtual void onPause();
    virtual void onResume();

private:
    static AudioEngine* getInstance()
    {
        static WindowsAudioEngine ret = WindowsAudioEngine();
        return &ret;
    }
    
    std::unique_ptr<DirectX::AudioEngine> _audioEngine;
    
    WindowsAudioEngine();
    virtual ~WindowsAudioEngine();
    WindowsAudioEngine(const WindowsAudioEngine&);
    WindowsAudioEngine& operator=(const WindowsAudioEngine&);
};
