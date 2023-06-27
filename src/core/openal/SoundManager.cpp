//
//  SoundManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/24/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void SoundManager::prepare(std::vector<SoundDescriptor>& sds)
{
    for (auto& sd : sds)
    {
        _sounds.emplace(sd._soundID, Sound{sd});
    }
}

void SoundManager::loadData()
{
    for (auto& pair : _sounds)
    {
        Sound& s = pair.second;
        _loader.loadData(s);
    }
}

void SoundManager::loadIntoOpenALAndFreeData()
{
    for (auto& pair : _sounds)
    {
        Sound& s = pair.second;
        OAL.loadSound(s);
        _loader.freeData(s);
    }
}

void SoundManager::reset()
{
    for (auto& pair : _sounds)
    {
        Sound& s = pair.second;
        if (s._alHandles != nullptr)
        {
            OAL.unloadSound(s);
        }
        if (s._data != nullptr)
        {
            _loader.freeData(s);
        }
    }
    _sounds.clear();
}

Sound& SoundManager::sound(std::string soundID)
{
    auto q = _sounds.find(soundID);
    assert(q != _sounds.end());
    return q->second;
}

std::map<std::string, Sound>& SoundManager::sounds()
{
    return _sounds;
}

bool SoundManager::areSoundsLoaded()
{
    if (_sounds.empty())
    {
        return false;
    }
    
    for (auto& pair : _sounds)
    {
        Sound& s = pair.second;
        if (s._alHandles == nullptr)
        {
            return false;
        }
    }
    return true;
}
