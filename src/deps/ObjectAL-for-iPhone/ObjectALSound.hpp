//
//  ObjectALSound.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/PlatformMacros.hpp"
#include "core/audio/Sound.hpp"

#include <string>

#if IS_APPLE

class ObjectALSound : public Sound
{
public:
    ObjectALSound(std::string filePath, bool isMusic, float volume = 1.0f);
    virtual ~ObjectALSound();

    virtual void play();
    virtual void resume();
    virtual void pause();
    virtual void stop();
    virtual void setVolume(float volume);
    virtual bool isLooping();
    virtual bool isPlaying();
    virtual bool isPaused();

private:
    uint32_t _bufferKey;
};

#endif /* IS_APPLE */
