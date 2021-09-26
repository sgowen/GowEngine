//
//  AudioEngineFactory.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "deps/ObjectAL-for-iPhone/ObjectALAudioEngine.hpp"
#include "deps/oboe/OboeAudioEngine.hpp"
#include "deps/DirectXTK/DirectXTKAudioEngine.hpp"

#include "core/openal/OpenALAudioEngine.hpp"

#include <cassert>
#include <stdlib.h>

#define AUDIO_ENGINE AudioEngineFactory::getInstance()

class AudioEngineFactory
{
public:
    static GowAudioEngine& getInstance()
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
        #error Need to add GowAudioEngine implementation
#endif
    }
    
private:
    AudioEngineFactory();
    ~AudioEngineFactory();
    AudioEngineFactory(const AudioEngineFactory&);
    AudioEngineFactory& operator=(const AudioEngineFactory&);
};
