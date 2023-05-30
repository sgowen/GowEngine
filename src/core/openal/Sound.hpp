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
    Sound() : _isLooping(false) {}
    virtual ~Sound() {}
    
    virtual void play() = 0;
    virtual void resume() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void setVolume(float volume) = 0;
    virtual bool isLooping() = 0;
    virtual bool isPlaying() = 0;
    virtual bool isPaused() = 0;
    
    void setLooping(bool isLooping)
    {
        _isLooping = isLooping;
    }
    
protected:
    bool _isLooping;
};
