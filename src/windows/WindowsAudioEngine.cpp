//
//  WindowsAudioEngine.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "WindowsAudioEngine.hpp"

#include "SoundWrapper.hpp"
#include "WindowsSoundWrapper.hpp"
#include "PlatformMacros.hpp"

SoundWrapper* WindowsAudioEngine::loadSound(const char *filePath, uint8_t numInstances)
{
    return new WindowsSoundWrapper(filePath, _audioEngine.get(), numInstances);
}

SoundWrapper* WindowsAudioEngine::loadMusic(const char* filePath)
{
    return loadSound(1337, filePath);
}

void WindowsAudioEngine::pause()
{
    AudioEngine::pause();
    _audioEngine->Suspend();
}

void WindowsAudioEngine::resume()
{
    _audioEngine->Resume();
    AudioEngine::resume();
}

WindowsAudioEngine::WindowsAudioEngine() : AudioEngine()
{
	using namespace DirectX;

    AUDIO_ENGINE_FLAGS flags = AudioEngine_Default;
#if IS_DEBUG
    flags |= AudioEngine_Debug;
#endif
    _audioEngine = std::make_unique<AudioEngine>(flags);
}

WindowsAudioEngine::~WindowsAudioEngine()
{
    _audioEngine->Suspend();
}
