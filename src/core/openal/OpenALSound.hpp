//
//  OpenALSound.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/audio/Sound.hpp"
#include "core/common/PlatformMacros.hpp"

#if IS_LINUX

#include <AL/al.h>

class OpenALSound : public Sound
{
public:
    OpenALSound(std::string filePath, float volume = 1.0f);
    virtual ~OpenALSound();

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

#endif /* IS_LINUX */
