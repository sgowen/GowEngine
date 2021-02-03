//
//  LinuxSound.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Sound.hpp"

#include <AL/al.h>

class LinuxSound : public Sound
{
public:
    LinuxSound(uint16_t soundID, const char *filePath, float volume = 1.0f);
    virtual ~LinuxSound();

    virtual void play(bool isLooping = false);
    virtual void resume();
    virtual void pause();
    virtual void stop();
    virtual void setVolume(float volume);
    virtual bool isLooping();
    virtual bool isPlaying();
    virtual bool isPaused();

private:
    ALuint buf;
    ALuint src;
};
