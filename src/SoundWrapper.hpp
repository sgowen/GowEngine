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
    SoundWrapper(uint16_t soundID, int numInstances = 1);
    virtual ~SoundWrapper();

    Sound* getSoundInstance();
    std::vector<Sound *> getSounds();
    int getSoundIndex();
    int getSoundID();
    int getNumInstances();

protected:
    std::vector<Sound *> _sounds;
    int _soundIndex;
    int _soundID;
    int _numInstances;
};
