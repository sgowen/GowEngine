//
//  ObjectALSound.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

ObjectALSound::ObjectALSound(std::string filePath, bool isMusic, float volume) : Sound(),
_bufferKey(0)
{
    std::string bundleFilePath = AppleAssetHandler::getInstance().getBundleFilePath(filePath);
    if (isMusic)
    {
        loadMusic(bundleFilePath.c_str());
    }
    else
    {
        _bufferKey = loadSound(bundleFilePath.c_str());
    }
}

ObjectALSound::~ObjectALSound()
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

void ObjectALSound::play()
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

void ObjectALSound::resume()
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

void ObjectALSound::pause()
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

void ObjectALSound::stop()
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

void ObjectALSound::setVolume(float volume)
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

bool ObjectALSound::isLooping()
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

bool ObjectALSound::isPlaying()
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

bool ObjectALSound::isPaused()
{
    return !isPlaying();
}
