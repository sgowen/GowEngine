//
//  SoundManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/24/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "SoundWrapper.hpp"
#include "core/assets/SoundDescriptor.hpp"

#include <map>
#include <vector>
#include <string>

class SoundManager
{
public:
    void loadSounds(std::vector<SoundDescriptor>& soundDescriptors);
    void unloadSounds(std::vector<SoundDescriptor>& soundDescriptors);
    SoundWrapper* sound(uint16_t soundID);
    std::map<uint16_t, SoundWrapper*>& sounds();
    SoundWrapper* music();
    
private:
    std::map<uint16_t, SoundWrapper*> _sounds;
    SoundWrapper* _music;
    
    void loadSound(uint16_t soundID, std::string filePath, uint8_t numInstances = 1);
    void unloadSound(uint16_t soundID);
    void loadMusic(std::string filePath);
    void unloadMusic();
};
