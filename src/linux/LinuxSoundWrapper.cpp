//
//  LinuxSoundWrapper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "LinuxSoundWrapper.hpp"

#include "LinuxSound.hpp"

LinuxSoundWrapper::LinuxSoundWrapper(const char *filePath, uint8_t numInstances) : SoundWrapper(numInstances)
{
    for (int i = 0; i < _numInstances; ++i)
    {
        _sounds.push_back(new LinuxSound(filePath));
    }
}
