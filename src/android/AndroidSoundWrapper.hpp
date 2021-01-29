//
//  AndroidSoundWrapper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/21/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "SoundWrapper.hpp"

class SimpleMultiPlayer;

class AndroidSoundWrapper : public SoundWrapper
{
public:
    AndroidSoundWrapper(SimpleMultiPlayer* simpleMultiPlayer, uint16_t soundID, const char *filePath, int numInstances = 1);
    virtual ~AndroidSoundWrapper();
};
