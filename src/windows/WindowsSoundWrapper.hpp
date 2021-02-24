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
    WindowsSoundWrapper(const char *filePath, DirectX::AudioEngine* audioEngine, uint8_t numInstances = 1);
    virtual ~WindowsSoundWrapper();
    
private:
    std::unique_ptr<DirectX::SoundEffect> _sound;
};
