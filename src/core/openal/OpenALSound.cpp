//
//  OpenALSound.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_LINUX

#include <AL/alut.h>

OpenALSound::OpenALSound(std::string filePath, float volume) : Sound(),
_buf(0),
_src(0)
{
    _buf = alutCreateBufferFromFile(filePath.c_str());
    assert(_buf != AL_NONE);

    alGenSources(1, &_src);

    alSourcei(_src, AL_BUFFER, _buf);
    alSourcei(_src, AL_LOOPING, AL_FALSE);
}

OpenALSound::~OpenALSound()
{
    stop();

    alDeleteSources(1, &_src);
    alDeleteBuffers(1, &_buf);
}

void OpenALSound::play()
{
    alSourcei(_src, AL_LOOPING, _isLooping ? AL_TRUE : AL_FALSE);
    alSourcePlay(_src);
}

void OpenALSound::resume()
{
    if (isPaused())
    {
        alSourcePlay(_src);
    }
}

void OpenALSound::pause()
{
    if (isPlaying())
    {
        alSourcePause(_src);
    }
}

void OpenALSound::stop()
{
    alSourceStop(_src);
}

void OpenALSound::setVolume(float volume)
{
    alSourcef(_src, AL_GAIN, volume);
}

bool OpenALSound::isLooping()
{
    ALint isLooping;
    alGetSourcei(_src, AL_LOOPING, &isLooping);

    return isLooping;
}

bool OpenALSound::isPlaying()
{
    ALint state;
    alGetSourcei(_src, AL_SOURCE_STATE, &state);

    return state == AL_PLAYING;
}

bool OpenALSound::isPaused()
{
    return !isPlaying();
}

#endif /* IS_LINUX */
