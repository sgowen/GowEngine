//
//  SoundManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/24/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "OpenALSoundWrapper.hpp"
#include "core/assets/SoundDescriptor.hpp"

#include <map>
#include <vector>
#include <string>

class SoundManager
{
public:
    void loadSounds(std::vector<SoundDescriptor>& soundDescriptors);
    void unloadSounds(std::vector<SoundDescriptor>& soundDescriptors);
    OpenALSoundWrapper* sound(std::string soundID);
    std::map<std::string, OpenALSoundWrapper*>& sounds();
    OpenALSoundWrapper* music();
    
private:
    std::map<std::string, OpenALSoundWrapper*> _sounds;
    OpenALSoundWrapper* _music;
    
    void loadSound(std::string soundID, std::string filePath, uint8_t numInstances = 1);
    void unloadSound(std::string soundID);
    void loadMusic(std::string filePath);
    void unloadMusic();
};
