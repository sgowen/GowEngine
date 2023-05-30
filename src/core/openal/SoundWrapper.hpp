//
//  SoundWrapper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <vector>
#include <stdint.h>

class Sound;

class SoundWrapper
{
public:
    SoundWrapper(uint8_t numInstances = 1);
    virtual ~SoundWrapper();

    void play();
    void resume();
    void pause();
    void stop();
    void setVolume(float volume);
    bool isLooping();
    bool isPlaying();
    bool isPaused();
    void setLooping(bool isLooping);
    Sound* nextSoundInstance();
    Sound* soundInstance(uint8_t soundIndex);
    Sound* soundInstance();
    std::vector<Sound *>& getSounds();
    uint8_t numInstances();

protected:
    std::vector<Sound *> _sounds;
    uint8_t _soundIndex;
    uint8_t _numInstances;
};
