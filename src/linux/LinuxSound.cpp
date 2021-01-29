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

LinuxSound::LinuxSound(uint16_t soundID, const char *filePath, float volume) : Sound(soundID),
_volume(volume),
_isLooping(false),
_isPaused(false)
{
    alGenBuffers(1, &buf);
    alGenSources(1, &src);
    buf = alutCreateBufferFromFile(filePath);

    assert(buf != AL_NONE);

    alSourcei(src, AL_BUFFER, buf);
    alSourcei(src, AL_LOOPING, _isLooping ? AL_TRUE : AL_FALSE);
}

LinuxSound::~LinuxSound()
{
    alDeleteBuffers(1, &buf);
    alDeleteSources(1, &src);
}

void LinuxSound::play(bool isLooping)
{
    _isLooping = isLooping;
    _isPaused = false;

    alSourcei(src, AL_LOOPING, _isLooping ? AL_TRUE : AL_FALSE);

    alSourcePlay(src);
}

void LinuxSound::resume()
{
    if (_isPaused)
    {
        alSourcePlay(src);

        _isPaused = false;
    }
}

void LinuxSound::pause()
{
    if (isPlaying())
    {
        alSourcePause(src);

        _isPaused = true;
    }
}

void LinuxSound::stop()
{
    _isLooping = false;
    _isPaused = false;

    alSourceStop(src);
}

void LinuxSound::setVolume(float volume)
{
    _volume = volume;

    alSourcef(src, AL_GAIN, _volume);
}

bool LinuxSound::isLooping()
{
    return _isLooping;
}

bool LinuxSound::isPlaying()
{
    ALint state;
    alGetSourcei(src, AL_SOURCE_STATE, &state);

    return state == AL_PLAYING;
}

bool LinuxSound::isPaused()
{
    return _isPaused;
}
