//
//  DirectXTKSoundWrapper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/audio/SoundWrapper.hpp"
#include "core/common/PlatformMacros.hpp"

#if IS_WINDOWS

#include "Audio.h"

#include <memory>

class DirectXTKSoundWrapper : public SoundWrapper
{
public:
    DirectXTKSoundWrapper(std::string filePath, DirectX::AudioEngine* audioEngine, uint8_t numInstances = 1);
    virtual ~DirectXTKSoundWrapper();
    
private:
    std::unique_ptr<DirectX::SoundEffect> _sound;
};

#endif /* IS_WINDOWS */
