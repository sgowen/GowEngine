//
//  Sound.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/10/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

class Sound
{
public:
    Sound(uint16_t soundID);
    virtual ~Sound();
    
    virtual void play(bool isLooping = false) = 0;
    virtual void resume() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void setVolume(float volume) = 0;
    virtual bool isLooping() = 0;
    virtual bool isPlaying() = 0;
    virtual bool isPaused() = 0;
    
    uint16_t getSoundID();
    
protected:
    uint16_t _soundID;
};
