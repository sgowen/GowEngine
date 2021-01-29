//
//  DirectXSound.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/10/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Sound.hpp"

#include "Audio.h"

#include <memory>

class DirectXSound : public Sound
{
public:
    DirectXSound(uint16_t soundID, DirectX::SoundEffect& sound, float volume = 1.0f);
    virtual ~DirectXSound();

    virtual void play(bool isLooping = false);
    virtual void resume();
    virtual void pause();
    virtual void stop();
    virtual void setVolume(float volume);
    virtual bool isLooping();
    virtual bool isPlaying();
    virtual bool isPaused();

private:
    std::unique_ptr<DirectX::SoundEffectInstance> _soundEffectInstance;
    bool _isLooping;
    bool _isPaused;
};
