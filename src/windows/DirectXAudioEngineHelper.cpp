//
//  DirectXAudioEngineHelper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "DirectXAudioEngineHelper.hpp"

#include "SoundWrapper.hpp"

#include "DirectXSoundWrapper.hpp"
#include "PlatformMacros.hpp"

void DirectXAudioEngineHelper::pause()
{
    _audioEngine->Suspend();
}

void DirectXAudioEngineHelper::resume()
{
    _audioEngine->Resume();
}

SoundWrapper* DirectXAudioEngineHelper::loadSound(uint16_t soundID, const char *filePath, int numInstances)
{
    return new DirectXSoundWrapper(soundID, filePath, _audioEngine.get(), numInstances);
}

SoundWrapper* DirectXAudioEngineHelper::loadMusic(const char* filePath)
{
    return loadSound(1337, filePath);
}

DirectXAudioEngineHelper::DirectXAudioEngineHelper() : AudioEngineHelper()
{
	using namespace DirectX;

    AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#if IS_DEBUG
    eflags = eflags | AudioEngine_Debug;
#endif
    _audioEngine = std::make_unique<AudioEngine>(eflags);
}

DirectXAudioEngineHelper::~DirectXAudioEngineHelper()
{
    if (_audioEngine)
    {
        _audioEngine->Suspend();
    }
}
