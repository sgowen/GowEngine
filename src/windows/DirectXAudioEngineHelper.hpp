//
//  DirectXAudioEngineHelper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "AudioEngineHelper.hpp"

#include "Audio.h"

#include <memory>

class DirectXAudioEngineHelper : public AudioEngineHelper
{
    friend class AudioEngineHelperFactory;
    
public:
    virtual void pause();
    virtual void resume();
    virtual SoundWrapper* loadSound(uint16_t soundID, const char *filePath, int numInstances = 1);
    virtual SoundWrapper* loadMusic(const char* filePath);

private:
    std::unique_ptr<DirectX::AudioEngine> _audioEngine;
    
    DirectXAudioEngineHelper();
    virtual ~DirectXAudioEngineHelper();
    DirectXAudioEngineHelper(const DirectXAudioEngineHelper&);
    DirectXAudioEngineHelper& operator=(const DirectXAudioEngineHelper&);
};
