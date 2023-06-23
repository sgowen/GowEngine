//
//  OpenALOpenALSoundWrapper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

OpenALSoundWrapper::OpenALSoundWrapper(std::string filePath, uint8_t numInstances) :
_soundIndex(0),
_numInstances(numInstances)
{
    for (int i = 0; i < _numInstances; ++i)
    {
        _sounds.push_back(new OpenALSound(filePath));
    }
}

OpenALSoundWrapper::~OpenALSoundWrapper()
{
    STLUtil::cleanUpVectorOfPointers(_sounds);
}

OpenALSound* OpenALSoundWrapper::nextSoundInstance()
{
    ++_soundIndex;
    if (_soundIndex >= _numInstances)
    {
        _soundIndex = 0;
    }
    
    return soundInstance();
}

OpenALSound* OpenALSoundWrapper::soundInstance(uint8_t soundIndex)
{
    assert(soundIndex < _numInstances);
    
    return _sounds[soundIndex];
}

OpenALSound* OpenALSoundWrapper::soundInstance()
{
    return soundInstance(_soundIndex);
}

std::vector<OpenALSound*>& OpenALSoundWrapper::getSounds()
{
    return _sounds;
}

uint8_t OpenALSoundWrapper::numInstances()
{
    return _numInstances;
}
