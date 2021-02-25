//
//  AudioEngineFactory.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "PlatformMacros.hpp"

#if IS_APPLE
    #include "apple/AppleAudioEngine.hpp"
#elif IS_ANDROID
    #include "android/AndroidAudioEngine.hpp"
#elif IS_LINUX
    #include "linux/LinuxAudioEngine.hpp"
#elif IS_WINDOWS
    #include "windows/WindowsAudioEngine.hpp"
#else
    #error Need to add AudioEngine implementation
#endif

#include <assert.h>
#include <stdlib.h>

#define AUDIO_ENGINE AudioEngineFactory::getInstance()

class AudioEngineFactory
{
public:
    static AudioEngine& getInstance()
    {
#if IS_APPLE
        return AppleAudioEngine::getInstance();
#elif IS_ANDROID
        return AndroidAudioEngine::getInstance();
#elif IS_LINUX
        return LinuxAudioEngine::getInstance();
#elif IS_WINDOWS
        return WindowsAudioEngine::getInstance();
#else
        #error Need to add AudioEngine implementation
#endif
    }
    
private:
    AudioEngineFactory();
    ~AudioEngineFactory();
    AudioEngineFactory(const AudioEngineFactory&);
    AudioEngineFactory& operator=(const AudioEngineFactory&);
};
