//
//  DirectXSoundWrapper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "SoundWrapper.hpp"

#include "Audio.h"

#include <memory>

class DirectXSoundWrapper : public SoundWrapper
{
public:
    DirectXSoundWrapper(uint16_t soundID, const char *filePath, DirectX::AudioEngine* audioEngine, int numInstances = 1);
    virtual ~DirectXSoundWrapper();
    
private:
    std::unique_ptr<DirectX::SoundEffect> _sound;
};
