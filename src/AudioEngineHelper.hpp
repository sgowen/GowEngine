//
//  AudioEngineHelper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

class SoundWrapper;

class AudioEngineHelper
{
    friend class AudioEngineHelperFactory;
    
public:
    virtual SoundWrapper* loadSound(const char *path, uint8_t numInstances = 1) = 0;
    virtual SoundWrapper* loadMusic(const char* path) = 0;
    virtual void pause() {}
    virtual void resume() {}
    
    bool isMusicDisabled()
    {
        return _musicDisabled;
    }

    void setMusicDisabled(bool value)
    {
        _musicDisabled = value;
    }

    bool areSoundsDisabled()
    {
        return _soundsDisabled;
    }

    void setSoundsDisabled(bool value)
    {
        _soundsDisabled = value;
    }
    
protected:
    AudioEngineHelper() {}
    virtual ~AudioEngineHelper() {}
    
private:
    bool _musicDisabled;
    bool _soundsDisabled;
};
