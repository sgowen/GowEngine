//
//  OpenALUtil.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/11/23.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

bool OpenALUtil::openDeviceAndCreateContext()
{
    const ALCchar *name;
    ALCdevice *device;
    ALCcontext *ctx;

    device = alcOpenDevice(nullptr);
    
    if (!device)
    {
        LOG("Could not open a device!");
        return false;
    }

    ctx = alcCreateContext(device, nullptr);
    if (ctx == nullptr || alcMakeContextCurrent(ctx) == ALC_FALSE)
    {
        if (ctx != nullptr)
        {
            alcDestroyContext(ctx);
        }
        alcCloseDevice(device);
        LOG("Could not set a context!");
        return false;
    }

    name = nullptr;
    if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
    {
        name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
    }
    
    if (!name || alcGetError(device) != AL_NO_ERROR)
    {
        name = alcGetString(device, ALC_DEVICE_SPECIFIER);
    }
    
    LOG("Opened \"%s\"", name);
    
    return true;
}

bool OpenALUtil::destroyContextAndCloseDevice()
{
    ALCdevice *device;
    ALCcontext *ctx;

    ctx = alcGetCurrentContext();
    if (ctx == nullptr)
    {
        return false;
    }
    
    device = alcGetContextsDevice(ctx);

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(ctx);
    alcCloseDevice(device);
    
    return true;
}

void OpenALUtil::create(OpenALSound& sound)
{
    alGenSources(1, &sound._src);

    alSourcei(sound._src, AL_BUFFER, sound._buf);
    alSourcei(sound._src, AL_LOOPING, AL_FALSE);
}

void OpenALUtil::destroy(OpenALSound& sound)
{
    stop(sound);

    alDeleteSources(1, &sound._src);
    ALenum err = alGetError();
    if (err != AL_NO_ERROR)
    {
        LOG("OpenAL Error: %s", alGetString(err));
    }
    
    alDeleteBuffers(1, &sound._buf);
    err = alGetError();
    if (err != AL_NO_ERROR)
    {
        LOG("OpenAL Error: %s", alGetString(err));
    }
}

void OpenALUtil::play(OpenALSound& sound)
{
    alSourcei(sound._src, AL_LOOPING, sound._isLooping ? AL_TRUE : AL_FALSE);
    alSourcePlay(sound._src);
}

void OpenALUtil::resume(OpenALSound& sound)
{
    if (isPaused(sound))
    {
        alSourcePlay(sound._src);
    }
}

void OpenALUtil::pause(OpenALSound& sound)
{
    if (isPlaying(sound))
    {
        alSourcePause(sound._src);
    }
}

void OpenALUtil::stop(OpenALSound& sound)
{
    alSourceStop(sound._src);
}

void OpenALUtil::setVolume(OpenALSound& sound, float volume)
{
    alSourcef(sound._src, AL_GAIN, volume);
}

bool OpenALUtil::isLooping(OpenALSound& sound)
{
    ALint isLooping;
    alGetSourcei(sound._src, AL_LOOPING, &isLooping);

    return isLooping;
}

bool OpenALUtil::isPlaying(OpenALSound& sound)
{
    ALint state;
    alGetSourcei(sound._src, AL_SOURCE_STATE, &state);

    return state == AL_PLAYING;
}

bool OpenALUtil::isPaused(OpenALSound& sound)
{
    return !isPlaying(sound);
}

void OpenALUtil::setLooping(OpenALSound& sound, bool isLooping)
{
    sound._isLooping = isLooping;
    alSourcei(sound._src, AL_LOOPING, sound._isLooping ? AL_TRUE : AL_FALSE);
}
