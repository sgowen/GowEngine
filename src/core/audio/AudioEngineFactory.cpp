//
//  AudioEngineFactory.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

GowAudioEngine& AudioEngineFactory::getInstance()
{
#if IS_APPLE
    return OpenALAudioEngine::getInstance();
#elif IS_ANDROID
    return OpenALAudioEngine::getInstance();
#elif IS_LINUX
    return OpenALAudioEngine::getInstance();
#elif IS_WINDOWS
    return DirectXTKAudioEngine::getInstance();
#else
        #error Need to add GowAudioEngine implementation
#endif
}
