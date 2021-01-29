//
//  NullSoundWrapper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "NullSoundWrapper.hpp"

#include "NullSound.hpp"

NullSoundWrapper::NullSoundWrapper(uint16_t soundID, const char *path, int numInstances) : SoundWrapper(soundID, numInstances)
{
    for (int i = 0; i < _numInstances; ++i)
    {
        _sounds.push_back(new NullSound(soundID, path));
    }
}

NullSoundWrapper::~NullSoundWrapper()
{
    // Empty
}
