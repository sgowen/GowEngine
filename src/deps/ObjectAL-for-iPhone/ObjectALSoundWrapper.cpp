//
//  ObjectALSoundWrapper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "ObjectALSoundWrapper.hpp"

#include "ObjectALSound.hpp"

ObjectALSoundWrapper::ObjectALSoundWrapper(std::string filePath, bool isMusic, uint8_t numInstances) : SoundWrapper(numInstances)
{
    for (int i = 0; i < _numInstances; ++i)
    {
        _sounds.push_back(new ObjectALSound(filePath, isMusic));
    }
}
