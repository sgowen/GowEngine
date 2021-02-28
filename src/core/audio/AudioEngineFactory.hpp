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
    #include "deps/ObjectAL-for-iPhone/ObjectALAudioEngine.hpp"
#elif IS_ANDROID
    #include "deps/oboe/OboeAudioEngine.hpp"
#elif IS_LINUX
    #include "core/openal/OpenALAudioEngine.hpp"
#elif IS_WINDOWS
    #include "deps/DirectXTK/DirectXTKAudioEngine.hpp"
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
        return ObjectALAudioEngine::getInstance();
#elif IS_ANDROID
        return OboeAudioEngine::getInstance();
#elif IS_LINUX
        return OpenALAudioEngine::getInstance();
#elif IS_WINDOWS
        return DirectXTKAudioEngine::getInstance();
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
