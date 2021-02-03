//
//  NullSound.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "NullSound.hpp"

NullSound::NullSound(uint16_t soundID, const char *path, float volume) : Sound(soundID)
{
    // Empty
}

void NullSound::play(bool isLooping)
{
    // Empty
}

void NullSound::resume()
{
    // Empty
}

void NullSound::pause()
{
    // Empty
}

void NullSound::stop()
{
    // Empty
}

void NullSound::setVolume(float volume)
{
    // Empty
}

bool NullSound::isLooping()
{
    return false;
}

bool NullSound::isPlaying()
{
    return false;
}

bool NullSound::isPaused()
{
    return false;
}
