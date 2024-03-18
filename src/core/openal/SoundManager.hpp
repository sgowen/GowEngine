//
//  SoundManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/24/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "deps/libsndfile/SoundLoader.hpp"
#include "Sound.hpp"

#include <map>
#include <string>

class SoundManager
{
public:
    void loadData(std::map<std::string, SoundDescriptor>& sds);
    void loadIntoOpenALAndFreeData(std::map<std::string, SoundDescriptor>& sds);
    void reset();
    Sound& sound(std::string soundID);
    std::map<std::string, Sound>& sounds();
    bool areSoundsLoaded();
    
private:
    SoundLoader _loader;
    std::map<std::string, Sound> _sounds;
    
    void loadSound(std::string soundID, std::string filePath, uint8_t numInstances = 1);
    void unloadSound(std::string soundID);
};
