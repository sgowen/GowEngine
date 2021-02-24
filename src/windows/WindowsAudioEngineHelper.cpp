//
//  WindowsAudioEngineHelper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "WindowsAudioEngineHelper.hpp"

#include "SoundWrapper.hpp"
#include "WindowsSoundWrapper.hpp"
#include "PlatformMacros.hpp"

SoundWrapper* WindowsAudioEngineHelper::loadSound(const char *filePath, uint8_t numInstances)
{
    return new WindowsSoundWrapper(filePath, _audioEngine.get(), numInstances);
}

SoundWrapper* WindowsAudioEngineHelper::loadMusic(const char* filePath)
{
    return loadSound(1337, filePath);
}

void WindowsAudioEngineHelper::pause()
{
    _audioEngine->Suspend();
}

void WindowsAudioEngineHelper::resume()
{
    _audioEngine->Resume();
}

WindowsAudioEngineHelper::WindowsAudioEngineHelper() : AudioEngineHelper()
{
	using namespace DirectX;

    AUDIO_ENGINE_FLAGS flags = AudioEngine_Default;
#if IS_DEBUG
    flags |= AudioEngine_Debug;
#endif
    _audioEngine = std::make_unique<AudioEngine>(flags);
}

WindowsAudioEngineHelper::~WindowsAudioEngineHelper()
{
    _audioEngine->Suspend();
}
