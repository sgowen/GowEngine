//
//  WindowsSoundWrapper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "SoundWrapper.hpp"

#include "Audio.h"

#include <memory>

class WindowsSoundWrapper : public SoundWrapper
{
public:
    WindowsSoundWrapper(uint16_t soundID, const char *filePath, Windows::AudioEngine* audioEngine, int numInstances = 1);
    virtual ~WindowsSoundWrapper();
    
private:
    std::unique_ptr<Windows::SoundEffect> _sound;
};
