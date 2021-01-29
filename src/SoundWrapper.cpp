//
//  SoundWrapper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 10/20/16.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "SoundWrapper.hpp"

#include "Sound.hpp"
#include "GowUtil.hpp"

SoundWrapper::SoundWrapper(uint16_t soundID, int numInstances) :
_soundIndex(0),
_soundID(soundID),
_numInstances(numInstances)
{
    // Empty
}

SoundWrapper::~SoundWrapper()
{
    GowUtil::cleanUpVectorOfPointers(_sounds);
}

Sound* SoundWrapper::getSoundInstance()
{
    Sound* ret = _sounds[_soundIndex++];
    if (_soundIndex >= _numInstances)
    {
        _soundIndex = 0;
    }
    
    return ret;
}

std::vector<Sound *> SoundWrapper::getSounds()
{
    return _sounds;
}

int SoundWrapper::getSoundIndex()
{
    return _soundIndex;
}

int SoundWrapper::getSoundID()
{
    return _soundID;
}

int SoundWrapper::getNumInstances()
{
    return _numInstances;
}
