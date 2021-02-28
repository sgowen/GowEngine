//
//  OpenALAudioEngine.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/21/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "PlatformMacros.hpp"
#if IS_LINUX

#include "OpenALAudioEngine.hpp"

#include "SoundWrapper.hpp"
#include "OpenALSoundWrapper.hpp"

#include <AL/alut.h>
#include <stdio.h>

SoundWrapper* OpenALAudioEngine::loadSound(std::string filePath, uint8_t numInstances)
{
    return new OpenALSoundWrapper(filePath, numInstances);
}

SoundWrapper* OpenALAudioEngine::loadMusic(std::string filePath)
{
    return loadSound(1337, filePath);
}

static void reportError()
{
    fprintf(stderr, "ALUT error: %s\n", alutGetErrorString(alutGetError()));
}

OpenALAudioEngine::OpenALAudioEngine() : AudioEngine()
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
