//
//  AudioEngineHelperFactory.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "AudioEngineHelperFactory.hpp"

#include "PlatformMacros.hpp"

#if IS_APPLE
    #include "apple/AppleAudioEngineHelper.hpp"
#elif IS_ANDROID
    #include "android/AndroidAudioEngineHelper.hpp"
#elif IS_LINUX
    #include "linux/LinuxAudioEngineHelper.hpp"
#elif IS_WINDOWS
    #include "windows/DirectXAudioEngineHelper.hpp"
#else
    #include "NullAudioEngineHelper.hpp"
#endif

#include <assert.h>
#include <stdlib.h>

AudioEngineHelper* AudioEngineHelperFactory::create()
{
#if IS_APPLE
    return new AppleAudioEngineHelper();
#elif IS_ANDROID
    return new AndroidAudioEngineHelper();
#elif IS_LINUX
    return new LinuxAudioEngineHelper();
#elif IS_WINDOWS
    return new DirectXAudioEngineHelper();
#else
    return new NullAudioEngineHelper();
#endif
}

void AudioEngineHelperFactory::destroy(AudioEngineHelper* aeh)
{
    assert(aeh != NULL);

    delete aeh;
}
