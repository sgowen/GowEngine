//
//  SoundDescriptor.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/18.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>

struct SoundDescriptor
{
    const std::string _soundID;
    const std::string _filePath;
    const uint8_t _numInstances;
    
    SoundDescriptor(std::string soundID, std::string filePath, uint8_t numInstances) :
    _soundID(soundID),
    _filePath(filePath),
    _numInstances(numInstances)
    {
        // Empty
    }
};
