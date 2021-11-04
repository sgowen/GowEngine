//
//  DirectXTKSoundWrapper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildConstants.hpp>
#if IS_WINDOWS

#include "core/audio/SoundWrapper.hpp"

#include <memory>

namespace DirectX
{
    class AudioEngine;
    class SoundEffect;
};

class DirectXTKSoundWrapper : public SoundWrapper
{
public:
    DirectXTKSoundWrapper(std::string filePath, DirectX::AudioEngine* audioEngine, uint8_t numInstances = 1);
    virtual ~DirectXTKSoundWrapper();
    
private:
    std::unique_ptr<DirectX::SoundEffect> _sound;
};

#endif /* IS_WINDOWS */
