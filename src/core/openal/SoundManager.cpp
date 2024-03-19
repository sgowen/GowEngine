//
//  SoundManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/24/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void SoundManager::loadData(std::map<std::string, SoundDescriptor>& sds)
{
    for (auto& pair : sds)
    {
        _sounds.emplace(pair.first, Sound{pair.second});
        Sound& s = sound(pair.first);
        _loader.loadData(s);
    }
}

void SoundManager::loadIntoOpenALAndFreeData(std::map<std::string, SoundDescriptor>& sds)
{
    for (auto& pair : sds)
    {
        Sound& s = sound(pair.first);
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
