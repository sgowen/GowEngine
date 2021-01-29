//
//  LinuxAudioEngineHelper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/21/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "LinuxAudioEngineHelper.hpp"

#include "SoundWrapper.hpp"

#include "LinuxSoundWrapper.hpp"

#include <AL/alut.h>
#include <stdio.h>

SoundWrapper* LinuxAudioEngineHelper::loadSound(uint16_t soundID, const char *filePath, int numInstances)
{
    return new LinuxSoundWrapper(soundID, filePath, numInstances);
}

SoundWrapper* LinuxAudioEngineHelper::loadMusic(const char* filePath)
{
    return loadSound(1337, filePath);
}

static void reportError()
{
    fprintf(stderr, "ALUT error: %s\n", alutGetErrorString(alutGetError()));
}

LinuxAudioEngineHelper::LinuxAudioEngineHelper() : AudioEngineHelper()
{
    if (!alutInit(NULL, NULL))
    {
        reportError();
    }
}

LinuxAudioEngineHelper::~LinuxAudioEngineHelper()
{
    if (!alutExit())
    {
        reportError();
    }
}
