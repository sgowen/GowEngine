//
//  OpenALSoundWrapper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_LINUX || IS_APPLE || IS_ANDROID

OpenALSoundWrapper::OpenALSoundWrapper(std::string filePath, uint8_t numInstances) : SoundWrapper(numInstances)
{
    for (int i = 0; i < _numInstances; ++i)
    {
        _sounds.push_back(new OpenALSound(filePath));
    }
}

#endif /* IS_LINUX */
