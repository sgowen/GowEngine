//
//  AppleSound.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "AppleSound.hpp"

#include "ObjectALWrapper.hpp"

AppleSound::AppleSound(uint16_t soundID, const char *filePath, float volume) : Sound(soundID),
_isMusic(soundID == 1337)
{
    if (_isMusic)
    {
        loadMusic(filePath);
    }
    else
    {
        loadSound(soundID, filePath);
    }
}

AppleSound::~AppleSound()
{
    stop();
    
    if (_isMusic)
    {
        unloadMusic();
    }
    else
    {
        unloadSound(_soundID);
    }
}

void AppleSound::play(bool isLooping)
{
    if (_isMusic)
    {
        stopMusic();
        playMusic(isLooping);
    }
    else
    {
        stopSound(_soundID);
        playSound(_soundID, isLooping);
    }
}

void AppleSound::resume()
{
    if (isPaused())
    {
        if (_isMusic)
        {
            resumeMusic();
        }
        else
        {
            resumeSound(_soundID);
        }
    }
}

void AppleSound::pause()
{
    if (isPlaying())
    {
        if (_isMusic)
        {
            pauseMusic();
        }
        else
        {
            pauseSound(_soundID);
        }
    }
}

void AppleSound::stop()
{
    if (_isMusic)
    {
        stopMusic();
    }
    else
    {
        stopSound(_soundID);
    }
}

void AppleSound::setVolume(float volume)
{
    if (_isMusic)
    {
        setMusicVolume(volume);
    }
    else
    {
        setSoundVolume(_soundID, volume);
    }
}

bool AppleSound::isLooping()
{
    if (_isMusic)
    {
        return isMusicLooping();
    }
    else
    {
        return isSoundLooping(_soundID);
    }
}

bool AppleSound::isPlaying()
{
    if (_isMusic)
    {
        return isMusicPlaying();
    }
    else
    {
        return isSoundPlaying(_soundID);
    }
}

bool AppleSound::isPaused()
{
    return !isPlaying();
}
