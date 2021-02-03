//
//  WindowsSound.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/10/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Sound.hpp"

#include "Audio.h"

#include <memory>

class WindowsSound : public Sound
{
public:
    WindowsSound(uint16_t soundID, Windows::SoundEffect& sound, float volume = 1.0f);
    virtual ~WindowsSound();

    virtual void play(bool isLooping = false);
    virtual void resume();
    virtual void pause();
    virtual void stop();
    virtual void setVolume(float volume);
    virtual bool isLooping();
    virtual bool isPlaying();
    virtual bool isPaused();

private:
    std::unique_ptr<Windows::SoundEffectInstance> _soundEffectInstance;
};
