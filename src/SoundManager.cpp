//
//  SoundManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/24/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "SoundManager.hpp"

#include "SoundDescriptor.hpp"
#include "AudioEngineHelperFactory.hpp"

void SoundManager::loadSounds(std::vector<SoundDescriptor>& soundDescriptors)
{
    for (SoundDescriptor& sd : soundDescriptors)
    {
        uint16_t soundID = sd._soundID;
        if (soundID == 1337)
        {
            loadMusic(sd._filePath.c_str());
        }
        else
        {
            loadSound(soundID, sd._filePath.c_str(), sd._numInstances);
        }
    }
}

void SoundManager::unloadSounds(std::vector<SoundDescriptor>& soundDescriptors)
{
    for (SoundDescriptor& sd : soundDescriptors)
    {
        uint16_t soundID = sd._soundID;
        if (soundID == 1337)
        {
            unloadMusic();
        }
        else
        {
            unloadSound(soundID);
        }
    }
}

SoundWrapper* SoundManager::sound(uint16_t soundID)
{
    SoundWrapper* ret = NULL;
    
    auto q = _sounds.find(soundID);
    if (q != _sounds.end())
    {
        ret = q->second;
    }
    
    return ret;
}

void SoundManager::loadSound(uint16_t soundID, const char *path, uint8_t numInstances)
{
    assert(_sounds.find(soundID) == _sounds.end());
    
    SoundWrapper* sw = AUDIO_ENGINE_HELPER.loadSound(path, numInstances);
    _sounds.emplace(soundID, sw);
}

void SoundManager::unloadSound(uint16_t soundID)
{
    auto q = _sounds.find(soundID);
    assert(q != _sounds.end());
    delete q->second;
    _sounds.erase(q);
}

void SoundManager::loadMusic(const char *path)
{
    unloadMusic();
    
    _music = AUDIO_ENGINE_HELPER.loadMusic(path);
}

void SoundManager::unloadMusic()
{
    if (_music == NULL)
    {
        return;
    }
    
    delete _music;
    _music = NULL;
}
