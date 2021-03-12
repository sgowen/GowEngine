//
//  DirectXTKSound.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/10/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>
#if IS_WINDOWS

DirectXTKSound::DirectXTKSound(DirectX::SoundEffect& sound, float volume) : Sound()
{
    _soundEffectInstance = sound.CreateInstance();
}

DirectXTKSound::~DirectXTKSound()
{
    stop();
    
    _soundEffectInstance.reset();
}

void DirectXTKSound::play()
{
	_soundEffectInstance->Stop();
    _soundEffectInstance->Play(_isLooping);
}

void DirectXTKSound::resume()
{
    if (isPaused())
    {
        _soundEffectInstance->Resume();
    }
}

void DirectXTKSound::pause()
{
    if (isPlaying())
    {
        _soundEffectInstance->Pause();
    }
}

void DirectXTKSound::stop()
{    
    _soundEffectInstance->Stop();
}

void DirectXTKSound::setVolume(float volume)
{
    _soundEffectInstance->SetVolume(volume);
}

bool DirectXTKSound::isLooping()
{
    return _soundEffectInstance->IsLooped();
}

bool DirectXTKSound::isPlaying()
{
	return _soundEffectInstance->GetState() == DirectX::SoundState::PLAYING;
}

bool DirectXTKSound::isPaused()
{
    return !isPlaying();
}

#endif /* IS_WINDOWS */
