//
//  NullSound.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "NullSound.hpp"

NullSound::NullSound(uint16_t soundID, const char *path, float volume) : Sound(soundID),
_volume(volume),
_isLooping(false),
_isPaused(false)
{
    // Empty
}

NullSound::~NullSound()
{
    // Empty
}

void NullSound::play(bool isLooping)
{
    _isLooping = isLooping;
    _isPaused = false;
    
    // Empty
}

void NullSound::resume()
{
    if (_isPaused)
    {
        // Empty
        
        _isPaused = false;
    }
}

void NullSound::pause()
{
    if (isPlaying())
    {
        // Empty
        
        _isPaused = true;
    }
}

void NullSound::stop()
{
    _isLooping = false;
    _isPaused = false;
    
    // Empty
}

void NullSound::setVolume(float volume)
{
    _volume = volume;
    
    // Empty
}

bool NullSound::isLooping()
{
    return _isLooping;
}

bool NullSound::isPlaying()
{
    return false;
}

bool NullSound::isPaused()
{
    return _isPaused;
}
