//
//  OpenALSoundWrapper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <vector>
#include <stdint.h>

class OpenALSound;

class OpenALSoundWrapper
{
public:
    OpenALSoundWrapper(std::string filePath, uint8_t numInstances = 1);
    ~OpenALSoundWrapper();
    
    OpenALSound* nextSoundInstance();
    OpenALSound* soundInstance(uint8_t soundIndex);
    OpenALSound* soundInstance();
    std::vector<OpenALSound*>& getSounds();
    uint8_t numInstances();
    
private:
    std::vector<OpenALSound*> _sounds;
    uint8_t _soundIndex;
    uint8_t _numInstances;
};
