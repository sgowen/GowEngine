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
_objectALSoundIndex(-1),
_isMusic(soundID == 1337)
{
    if (_isMusic)
    {
        loadMusic(filePath);
    }
    else
    {
        _objectALSoundIndex = loadSound(filePath);
    }
}

AppleSound::~AppleSound()
{
    if (_isMusic)
    {
        stopMusic();
    }
    else
    {
        stopSound(_objectALSoundIndex);
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
        stopSound(_objectALSoundIndex);
        playSound(_objectALSoundIndex, isLooping);
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
            resumeSound(_objectALSoundIndex);
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
            pauseSound(_objectALSoundIndex);
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
        stopSound(_objectALSoundIndex);
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
        setSoundVolume(_objectALSoundIndex, volume);
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
        return isSoundLooping(_objectALSoundIndex);
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
        return isSoundPlaying(_objectALSoundIndex);
    }
}

bool AppleSound::isPaused()
{
    return !isPlaying();
}
