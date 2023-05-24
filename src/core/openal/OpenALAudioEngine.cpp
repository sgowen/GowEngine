//
//  OpenALAudioEngine.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/21/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_LINUX || IS_APPLE

#include "alhelpers.h"

SoundWrapper* OpenALAudioEngine::loadSound(std::string filePath, uint8_t numInstances)
{
    return new OpenALSoundWrapper(filePath, numInstances);
}

SoundWrapper* OpenALAudioEngine::loadMusic(std::string filePath)
{
    return loadSound(filePath);
}

OpenALAudioEngine::OpenALAudioEngine() : GowAudioEngine()
{
    if (!InitAL(nullptr, nullptr))
    {
        // wtf
    }
}

OpenALAudioEngine::~OpenALAudioEngine()
{
    CloseAL();
}

#endif /* IS_LINUX */
