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
    LinuxSound(const char *filePath, float volume = 1.0f);
    virtual ~LinuxSound();

    virtual void play();
    virtual void resume();
    virtual void pause();
    virtual void stop();
    virtual void setVolume(float volume);
    virtual bool isLooping();
    virtual bool isPlaying();
    virtual bool isPaused();

private:
    ALuint _buf;
    ALuint _src;
};
