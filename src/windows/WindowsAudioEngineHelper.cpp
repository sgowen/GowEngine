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

void WindowsAudioEngineHelper::pause()
{
    _audioEngine->Suspend();
}

void WindowsAudioEngineHelper::resume()
{
    _audioEngine->Resume();
}

SoundWrapper* WindowsAudioEngineHelper::loadSound(uint16_t soundID, const char *filePath, int numInstances)
{
    return new WindowsSoundWrapper(soundID, filePath, _audioEngine.get(), numInstances);
}

SoundWrapper* WindowsAudioEngineHelper::loadMusic(const char* filePath)
{
    return loadSound(1337, filePath);
}

WindowsAudioEngineHelper::WindowsAudioEngineHelper() : AudioEngineHelper()
{
	using namespace Windows;

    AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#if IS_DEBUG
    eflags = eflags | AudioEngine_Debug;
#endif
    _audioEngine = std::make_unique<AudioEngine>(eflags);
}

WindowsAudioEngineHelper::~WindowsAudioEngineHelper()
{
    if (_audioEngine)
    {
        _audioEngine->Suspend();
    }
}
