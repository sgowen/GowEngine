//
//  SoundWrapper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 10/20/16.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "SoundWrapper.hpp"

#include "Sound.hpp"
#include "STLUtil.hpp"

SoundWrapper::SoundWrapper(uint8_t numInstances) :
_soundIndex(0),
_numInstances(numInstances)
{
    // Empty
}

SoundWrapper::~SoundWrapper()
{
    STLUtil::cleanUpVectorOfPointers(_sounds);
}

void SoundWrapper::play()
{
    soundInstance()->play();
}

void SoundWrapper::resume()
{
    soundInstance()->resume();
}

void SoundWrapper::pause()
{
    soundInstance()->pause();
}

void SoundWrapper::stop()
{
    soundInstance()->stop();
}

void SoundWrapper::setVolume(float volume)
{
    soundInstance()->setVolume(volume);
}

bool SoundWrapper::isLooping()
{
    return soundInstance()->isLooping();
}

bool SoundWrapper::isPlaying()
{
    return soundInstance()->isPlaying();
}

bool SoundWrapper::isPaused()
{
    return soundInstance()->isPaused();
}

void SoundWrapper::setLooping(bool isLooping)
{
    soundInstance()->setLooping(isLooping);
}

Sound* SoundWrapper::nextSoundInstance()
{
    ++_soundIndex;
    if (_soundIndex >= _numInstances)
    {
        _soundIndex = 0;
    }
    
    return soundInstance();
}

Sound* SoundWrapper::soundInstance(uint8_t soundIndex)
{
    assert(soundIndex < _numInstances);
    
    return _sounds[soundIndex];
}

Sound* SoundWrapper::soundInstance()
{
    return soundInstance(_soundIndex);
}

std::vector<Sound *> SoundWrapper::getSounds()
{
    return _sounds;
}

uint8_t SoundWrapper::numInstances()
{
    return _numInstances;
}
