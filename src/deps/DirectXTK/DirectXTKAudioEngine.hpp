//
//  DirectXTKAudioEngine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildMacros.hpp>
#if IS_WINDOWS

#include "core/audio/GowAudioEngine.hpp"

#include <memory>

namespace DirectX
{
    class AudioEngine;
};

class DirectXTKAudioEngine : public GowAudioEngine
{
    friend class AudioEngineFactory;
    
public:
    virtual SoundWrapper* loadSound(std::string filePath, uint8_t numInstances = 1);
    virtual SoundWrapper* loadMusic(std::string filePath);
    virtual void onPause();
    virtual void onResume();

private:
    static GowAudioEngine& getInstance()
    {
        static DirectXTKAudioEngine ret = DirectXTKAudioEngine();
        return ret;
    }
    
    std::unique_ptr<DirectX::AudioEngine> _audioEngine;
    
    DirectXTKAudioEngine();
    virtual ~DirectXTKAudioEngine();
    DirectXTKAudioEngine(const DirectXTKAudioEngine&);
    DirectXTKAudioEngine& operator=(const DirectXTKAudioEngine&);
};

#endif /* IS_WINDOWS */
