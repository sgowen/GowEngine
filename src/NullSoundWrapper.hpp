//
//  NullSoundWrapper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "SoundWrapper.hpp"

class NullSoundWrapper : public SoundWrapper
{
public:
    NullSoundWrapper(uint16_t soundID, const char *path, int numInstances = 1);
    virtual ~NullSoundWrapper();
};
