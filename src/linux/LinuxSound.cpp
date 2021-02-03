//
//  LinuxSound.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "LinuxSound.hpp"

#include <assert.h>

#include <AL/alut.h>

LinuxSound::LinuxSound(uint16_t soundID, const char *filePath, float volume) : Sound(soundID)
{
    alGenBuffers(1, &buf);
    alGenSources(1, &src);
    buf = alutCreateBufferFromFile(filePath);

    assert(buf != AL_NONE);

    alSourcei(src, AL_BUFFER, buf);
    alSourcei(src, AL_LOOPING, AL_FALSE);
}

LinuxSound::~LinuxSound()
{
    alDeleteBuffers(1, &buf);
    alDeleteSources(1, &src);
}

void LinuxSound::play(bool isLooping)
{
    alSourcei(src, AL_LOOPING, isLooping ? AL_TRUE : AL_FALSE);

    alSourcePlay(src);
}

void LinuxSound::resume()
{
    if (isPaused())
    {
        alSourcePlay(src);
    }
}

void LinuxSound::pause()
{
    if (isPlaying())
    {
        alSourcePause(src);
    }
}

void LinuxSound::stop()
{
    alSourceStop(src);
}

void LinuxSound::setVolume(float volume)
{
    alSourcef(src, AL_GAIN, volume);
}

bool LinuxSound::isLooping()
{
    ALint isLooping;
    alGetSourcei(src, AL_LOOPING, &isLooping);
    
    return isLooping;
}

bool LinuxSound::isPlaying()
{
    ALint state;
    alGetSourcei(src, AL_SOURCE_STATE, &state);

    return state == AL_PLAYING;
}

bool LinuxSound::isPaused()
{
    return !isPlaying();
}
