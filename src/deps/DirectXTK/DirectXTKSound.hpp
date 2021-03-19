//
//  DirectXTKSound.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/10/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/audio/Sound.hpp"
#include "core/common/PlatformMacros.hpp"

#if IS_WINDOWS

#include "Audio.h"

#include <memory>

class DirectXTKSound : public Sound
{
public:
    DirectXTKSound(DirectX::SoundEffect& sound, float volume = 1.0f);
    virtual ~DirectXTKSound();

    virtual void play();
    virtual void resume();
    virtual void pause();
    virtual void stop();
    virtual void setVolume(float volume);
    virtual bool isLooping();
    virtual bool isPlaying();
    virtual bool isPaused();

private:
    std::unique_ptr<DirectX::SoundEffectInstance> _soundEffectInstance;
};

#endif /* IS_WINDOWS */
