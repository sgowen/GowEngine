//
//  SoundDescriptor.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>

struct SoundDescriptor
{
    const int _soundID;
    const std::string _filePath;
    const int _numInstances;
    
    SoundDescriptor(uint16_t soundID, std::string filePath, int numInstances) :
    _soundID(soundID),
    _filePath(filePath),
    _numInstances(numInstances)
    {
        // Empty
    }
};
