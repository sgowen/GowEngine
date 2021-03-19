//
//  OpenALAudioEngine.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/21/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_LINUX

#include <AL/alut.h>

SoundWrapper* OpenALAudioEngine::loadSound(std::string filePath, uint8_t numInstances)
{
    return new OpenALSoundWrapper(filePath, numInstances);
}

SoundWrapper* OpenALAudioEngine::loadMusic(std::string filePath)
{
    return loadSound(filePath);
}

static void reportError()
{
    fprintf(stderr, "ALUT error: %s\n", alutGetErrorString(alutGetError()));
}

OpenALAudioEngine::OpenALAudioEngine() : GowAudioEngine()
{
    if (!alutInit(NULL, NULL))
    {
        reportError();
    }
}

OpenALAudioEngine::~OpenALAudioEngine()
{
    if (!alutExit())
    {
        reportError();
    }
}

#endif /* IS_LINUX */
