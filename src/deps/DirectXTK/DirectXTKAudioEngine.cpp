//
//  DirectXTKAudioEngine.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_WINDOWS

#include "Audio.h"

SoundWrapper* DirectXTKAudioEngine::loadSound(std::string filePath, uint8_t numInstances)
{
    return new DirectXTKSoundWrapper(filePath, _audioEngine.get(), numInstances);
}

SoundWrapper* DirectXTKAudioEngine::loadMusic(std::string filePath)
{
    return loadSound(filePath);
}

void DirectXTKAudioEngine::onPause()
{
    _audioEngine->Suspend();
}

void DirectXTKAudioEngine::onResume()
{
    _audioEngine->Resume();
}

DirectXTKAudioEngine::DirectXTKAudioEngine() : GowAudioEngine()
{
	using namespace DirectX;

    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    AUDIO_ENGINE_FLAGS flags = AudioEngine_Default;
#if IS_DEBUG
    flags |= AudioEngine_Debug;
#endif
    _audioEngine = std::make_unique<DirectX::AudioEngine>(flags);
}

DirectXTKAudioEngine::~DirectXTKAudioEngine()
{
    _audioEngine->Suspend();
}

#endif /* IS_WINDOWS */
