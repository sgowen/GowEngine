//
//  SoundManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/24/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void SoundManager::loadSounds(std::vector<SoundDescriptor>& soundDescriptors)
{
    for (SoundDescriptor& sd : soundDescriptors)
    {
        std::string soundID = sd._soundID;
        if (soundID == "music")
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
        std::string soundID = sd._soundID;
        if (soundID == "music")
        {
            unloadMusic();
        }
        else
        {
            unloadSound(soundID);
        }
    }
}

OpenALSoundWrapper* SoundManager::sound(std::string soundID)
{
    OpenALSoundWrapper* ret = nullptr;
    
    auto q = _sounds.find(soundID);
    if (q != _sounds.end())
    {
        ret = q->second;
    }
    
    return ret;
}

std::map<std::string, OpenALSoundWrapper*>& SoundManager::sounds()
{
    return _sounds;
}

OpenALSoundWrapper* SoundManager::music()
{
    return _music;
}

void SoundManager::loadSound(std::string soundID, std::string filePath, uint8_t numInstances)
{
    assert(_sounds.find(soundID) == _sounds.end());
    
    OpenALSoundWrapper* sw = AUDIO_ENGINE.loadSound(filePath, numInstances);
    _sounds.emplace(soundID, sw);
}

void SoundManager::unloadSound(std::string soundID)
{
    if (ENGINE_CFG.fileLoggingEnabled())
    {
        // TODO, log the file path
        LOG("SoundManager::unloadSound");
    }
    
    auto q = _sounds.find(soundID);
    assert(q != _sounds.end());
    delete q->second;
    _sounds.erase(q);
}

void SoundManager::loadMusic(std::string filePath)
{
    unloadMusic();
    
    _music = AUDIO_ENGINE.loadMusic(filePath);
}

void SoundManager::unloadMusic()
{
    if (_music == nullptr)
    {
        return;
    }
    
    if (ENGINE_CFG.fileLoggingEnabled())
    {
        // TODO, log the file path
        LOG("SoundManager::unloadMusic");
    }
    
    delete _music;
    _music = nullptr;
}
