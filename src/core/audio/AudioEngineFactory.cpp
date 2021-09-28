//
//  AudioEngineFactory.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/BuildMacros.hpp>
#include <GowEngine/GowEngine.hpp>

GowAudioEngine& AudioEngineFactory::getInstance()
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
