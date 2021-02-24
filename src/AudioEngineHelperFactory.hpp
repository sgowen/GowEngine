//
//  AudioEngineHelperFactory.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "PlatformMacros.hpp"

#if IS_APPLE
    #include "apple/AppleAudioEngineHelper.hpp"
#elif IS_ANDROID
    #include "android/AndroidAudioEngineHelper.hpp"
#elif IS_LINUX
    #include "linux/LinuxAudioEngineHelper.hpp"
#elif IS_WINDOWS
    #include "windows/WindowsAudioEngineHelper.hpp"
#else
    #error Need to add AudioEngineHelper implementation
#endif

#include <assert.h>
#include <stdlib.h>

#define AUDIO_ENGINE_HELPER AudioEngineHelperFactory::getInstance()

class AudioEngineHelperFactory
{
public:
    static AudioEngineHelper* getInstance()
    {
#if IS_APPLE
        return AppleAudioEngineHelper::getInstance();
#elif IS_ANDROID
        return AndroidAudioEngineHelper::getInstance();
#elif IS_LINUX
        return LinuxAudioEngineHelper::getInstance();
#elif IS_WINDOWS
        return WindowsAudioEngineHelper::getInstance();
#else
        #error Need to add AudioEngineHelper implementation
#endif
    }
    
private:
    AudioEngineHelperFactory();
    ~AudioEngineHelperFactory();
    AudioEngineHelperFactory(const AudioEngineHelperFactory&);
    AudioEngineHelperFactory& operator=(const AudioEngineHelperFactory&);
};
