//
//  OpenALSoundWrapper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "PlatformMacros.hpp"
#if IS_LINUX

#include "OpenALSoundWrapper.hpp"

#include "OpenALSound.hpp"

OpenALSoundWrapper::OpenALSoundWrapper(std::string filePath, uint8_t numInstances) : SoundWrapper(numInstances)
{
    for (int i = 0; i < _numInstances; ++i)
    {
        _sounds.push_back(new OpenALSound(filePath));
    }
}

#endif /* IS_LINUX */
