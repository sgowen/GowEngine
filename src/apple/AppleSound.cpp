//
//  AppleSound.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "AppleSound.hpp"

#include "ObjectALWrapper.hpp"

AppleSound::AppleSound(const char *filePath, bool isMusic, float volume) : Sound(),
_bufferKey(0)
{
    if (isMusic)
    {
        loadMusic(filePath);
    }
    else
    {
        _bufferKey = loadSound(filePath);
    }
}

AppleSound::~AppleSound()
{
    stop();
    
    if (_bufferKey > 0)
    {
        unloadSound(_bufferKey);
    }
    else
    {
        unloadMusic();
    }
}

void AppleSound::play()
{
    if (_bufferKey > 0)
    {
        stopSound(_bufferKey);
        playSound(_bufferKey, _isLooping);
    }
    else
    {
        stopMusic();
        playMusic(_isLooping);
    }
}

void AppleSound::resume()
{
    if (isPaused())
    {
        if (_bufferKey > 0)
        {
            resumeSound(_bufferKey);
        }
        else
        {
            resumeMusic();
        }
    }
}

void AppleSound::pause()
{
    if (isPlaying())
    {
        if (_bufferKey > 0)
        {
            pauseSound(_bufferKey);
        }
        else
        {
            pauseMusic();
        }
    }
}

void AppleSound::stop()
{
    if (_bufferKey > 0)
    {
        stopSound(_bufferKey);
    }
    else
    {
        stopMusic();
    }
}

void AppleSound::setVolume(float volume)
{
    if (_bufferKey > 0)
    {
        setSoundVolume(_bufferKey, volume);
    }
    else
    {
        setMusicVolume(volume);
    }
}

bool AppleSound::isLooping()
{
    if (_bufferKey > 0)
    {
        return isSoundLooping(_bufferKey);
    }
    else
    {
        return isMusicLooping();
    }
}

bool AppleSound::isPlaying()
{
    if (_bufferKey > 0)
    {
        return isSoundPlaying(_bufferKey);
    }
    else
    {
        return isMusicPlaying();
    }
}

bool AppleSound::isPaused()
{
    return !isPlaying();
}
