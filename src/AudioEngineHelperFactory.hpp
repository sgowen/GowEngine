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

class AudioEngineHelperFactory
{
public:
    static AudioEngineHelper* create()
    {
#if IS_APPLE
        return new AppleAudioEngineHelper();
#elif IS_ANDROID
        return new AndroidAudioEngineHelper();
#elif IS_LINUX
        return new LinuxAudioEngineHelper();
#elif IS_WINDOWS
        return new WindowsAudioEngineHelper();
#else
        #error Need to add AudioEngineHelper implementation
#endif
    }
    
    static void destroy(AudioEngineHelper* aeh)
    {
        assert(aeh != NULL);

        delete aeh;
    }
    
private:
    AudioEngineHelperFactory();
    ~AudioEngineHelperFactory();
    AudioEngineHelperFactory(const AudioEngineHelperFactory&);
    AudioEngineHelperFactory& operator=(const AudioEngineHelperFactory&);
};
