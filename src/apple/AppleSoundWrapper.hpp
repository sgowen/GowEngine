//
//  AppleSoundWrapper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "SoundWrapper.hpp"

class AppleSoundWrapper : public SoundWrapper
{
public:
    AppleSoundWrapper(uint16_t soundID, const char *filePath, int numInstances = 1);
    virtual ~AppleSoundWrapper() {}
};
