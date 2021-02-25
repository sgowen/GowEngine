//
//  LinuxAudioEngine.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/21/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "LinuxAudioEngine.hpp"

#include "SoundWrapper.hpp"
#include "LinuxSoundWrapper.hpp"

#include <AL/alut.h>
#include <stdio.h>

SoundWrapper* LinuxAudioEngine::loadSound(const char *filePath, uint8_t numInstances)
{
    return new LinuxSoundWrapper(filePath, numInstances);
}

SoundWrapper* LinuxAudioEngine::loadMusic(const char* filePath)
{
    return loadSound(1337, filePath);
}

static void reportError()
{
    fprintf(stderr, "ALUT error: %s\n", alutGetErrorString(alutGetError()));
}

LinuxAudioEngine::LinuxAudioEngine() : AudioEngine()
{
    if (!alutInit(NULL, NULL))
    {
        reportError();
    }
}

LinuxAudioEngine::~LinuxAudioEngine()
{
    if (!alutExit())
    {
        reportError();
    }
}
