//
//  AppleSoundWrapper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "AppleSoundWrapper.hpp"

#include "AppleSound.hpp"

AppleSoundWrapper::AppleSoundWrapper(uint16_t soundID, const char *filePath, int numInstances) : SoundWrapper(soundID, numInstances)
{
    for (int i = 0; i < _numInstances; ++i)
    {
        _sounds.push_back(new AppleSound(soundID, filePath));
    }
}
