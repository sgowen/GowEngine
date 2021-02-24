//
//  LinuxSoundWrapper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "SoundWrapper.hpp"

class LinuxSoundWrapper : public SoundWrapper
{
public:
    LinuxSoundWrapper(const char *filePath, uint8_t numInstances = 1);
    virtual ~LinuxSoundWrapper() {}
};
