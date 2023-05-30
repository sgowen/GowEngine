//
//  OpenALAudioEngine.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/21/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"

/* InitAL opens a device and sets up a context using default attributes, making
 * the program ready to call OpenAL functions. */
int InitAL(char ***argv, int *argc)
{
    const ALCchar *name;
    ALCdevice *device;
    ALCcontext *ctx;

    /* Open and initialize a device */
    device = NULL;
    if (argc && argv && *argc > 1 && strcmp((*argv)[0], "-device") == 0)
    {
        device = alcOpenDevice((*argv)[1]);
        if (!device)
        {
            fprintf(stderr, "Failed to open \"%s\", trying default\n", (*argv)[1]);
        }
        (*argv) += 2;
        (*argc) -= 2;
    }
    
    if (!device)
    {
        device = alcOpenDevice(NULL);
    }
    
    if (!device)
    {
        fprintf(stderr, "Could not open a device!\n");
        return 1;
    }

    ctx = alcCreateContext(device, NULL);
    if (ctx == NULL || alcMakeContextCurrent(ctx) == ALC_FALSE)
    {
        if (ctx != NULL)
        {
            alcDestroyContext(ctx);
        }
        alcCloseDevice(device);
        fprintf(stderr, "Could not set a context!\n");
        return 1;
    }

    name = NULL;
    if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
    {
        name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
    }
    
    if (!name || alcGetError(device) != AL_NO_ERROR)
    {
        name = alcGetString(device, ALC_DEVICE_SPECIFIER);
    }
    
    printf("Opened \"%s\"\n", name);

    return 0;
}

/* CloseAL closes the device belonging to the current context, and destroys the
 * context. */
void CloseAL(void)
{
    ALCdevice *device;
    ALCcontext *ctx;

    ctx = alcGetCurrentContext();
    if (ctx == NULL)
    {
        return;
    }

    device = alcGetContextsDevice(ctx);

    alcMakeContextCurrent(NULL);
    alcDestroyContext(ctx);
    alcCloseDevice(device);
}

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
