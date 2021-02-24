//
//  AndroidSoundWrapper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/21/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "AndroidSoundWrapper.hpp"

#include "SimpleMultiPlayer.hpp"
#include "AndroidSound.hpp"

AndroidSoundWrapper::AndroidSoundWrapper(SimpleMultiPlayer* simpleMultiPlayer, const char *filePath, uint8_t numInstances) : SoundWrapper(numInstances)
{
    for (int i = 0; i < _numInstances; ++i)
    {
        _sounds.push_back(new AndroidSound(simpleMultiPlayer, filePath));
    }
}
