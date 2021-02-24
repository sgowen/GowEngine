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

LinuxSound::LinuxSound(const char *filePath, float volume) : Sound(),
_buf(0),
_src(0)
{
    alGenSources(1, &_src);
    _buf = alutCreateBufferFromFile(filePath);
    assert(_buf != AL_NONE);
    alSourcei(_src, AL_BUFFER, _buf);
    alSourcei(_src, AL_LOOPING, AL_FALSE);
}

LinuxSound::~LinuxSound()
{
    stop();
    
    alDeleteBuffers(1, &_buf);
    alDeleteSources(1, &_src);
}

void LinuxSound::play()
{
    alSourcei(_src, AL_LOOPING, _isLooping ? AL_TRUE : AL_FALSE);

    alSourcePlay(_src);
}

void LinuxSound::resume()
{
    if (isPaused())
    {
        alSourcePlay(_src);
    }
}

void LinuxSound::pause()
{
    if (isPlaying())
    {
        alSourcePause(_src);
    }
}

void LinuxSound::stop()
{
    alSourceStop(_src);
}

void LinuxSound::setVolume(float volume)
{
    alSourcef(_src, AL_GAIN, volume);
}

bool LinuxSound::isLooping()
{
    ALint isLooping;
    alGetSourcei(_src, AL_LOOPING, &isLooping);
    
    return isLooping;
}

bool LinuxSound::isPlaying()
{
    ALint state;
    alGetSourcei(_src, AL_SOURCE_STATE, &state);

    return state == AL_PLAYING;
}

bool LinuxSound::isPaused()
{
    return !isPlaying();
}
