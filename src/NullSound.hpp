//
//  NullSound.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Sound.hpp"

class NullSound : public Sound
{
public:
    NullSound(uint16_t soundID, const char *path, float volume = 1.0f);
    virtual ~NullSound() {}

    virtual void play(bool isLooping = false);
    virtual void resume();
    virtual void pause();
    virtual void stop();
    virtual void setVolume(float volume);
    virtual bool isLooping();
    virtual bool isPlaying();
    virtual bool isPaused();
};
