//
//  DirectXTKAudioEngine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "PlatformMacros.hpp"
#if IS_WINDOWS

#include "AudioEngine.hpp"

#include "Audio.h"

#include <memory>

class DirectXTKAudioEngine : public AudioEngine
{
    friend class AudioEngineFactory;
    
public:
    virtual SoundWrapper* loadSound(std::string filePath, uint8_t numInstances = 1);
    virtual SoundWrapper* loadMusic(std::string filePath);
    virtual void onPause();
    virtual void onResume();

private:
    static AudioEngine* getInstance()
    {
        static DirectXTKAudioEngine ret = DirectXTKAudioEngine();
        return &ret;
    }
    
    std::unique_ptr<DirectX::AudioEngine> _audioEngine;
    
    DirectXTKAudioEngine();
    virtual ~DirectXTKAudioEngine();
    DirectXTKAudioEngine(const DirectXTKAudioEngine&);
    DirectXTKAudioEngine& operator=(const DirectXTKAudioEngine&);
};

#endif /* IS_WINDOWS */
