//
//  AndroidSound.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/21/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Sound.hpp"

class SimpleMultiPlayer;

class AndroidSound : public Sound
{
public:
    AndroidSound(SimpleMultiPlayer* simpleMultiPlayer, uint16_t soundID, const char *filePath, float volume = 1.0f);
    virtual ~AndroidSound() {}

    virtual void play(bool isLooping = false);
    virtual void resume();
    virtual void pause();
    virtual void stop();
    virtual void setVolume(float volume);
    virtual bool isLooping();
    virtual bool isPlaying();
    virtual bool isPaused();

private:
    SimpleMultiPlayer* _simpleMultiPlayer;
    
    float _volume;
    int _oboeSoundIndex;
    bool _isLooping;
    bool _isPaused;
};
