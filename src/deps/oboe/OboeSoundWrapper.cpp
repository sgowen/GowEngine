//
//  OboeSoundWrapper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/21/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_ANDROID

OboeSoundWrapper::OboeSoundWrapper(SimpleMultiPlayer* simpleMultiPlayer, std::string filePath, uint8_t numInstances) : SoundWrapper(numInstances)
{
    for (int i = 0; i < _numInstances; ++i)
    {
        _sounds.push_back(new OboeSound(simpleMultiPlayer, filePath));
    }
}

#endif /* IS_ANDROID */
