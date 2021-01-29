//
//  DirectXSound.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/10/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "DirectXSound.hpp"

DirectXSound::DirectXSound(uint16_t soundID, DirectX::SoundEffect& sound, float volume) : Sound(soundID),
_isLooping(false),
_isPaused(false)
{
    _soundEffectInstance = sound.CreateInstance();
}

DirectXSound::~DirectXSound()
{
    _soundEffectInstance->Pause();
    
    _soundEffectInstance.reset();
}

void DirectXSound::play(bool isLooping)
{
    _isLooping = isLooping;
    _isPaused = false;
 
	_soundEffectInstance->Stop();
    _soundEffectInstance->Play(isLooping);
}

void DirectXSound::resume()
{
    if (_isPaused)
    {
        _soundEffectInstance->Resume();
        
        _isPaused = false;
    }
}

void DirectXSound::pause()
{
    if (isPlaying())
    {
        _soundEffectInstance->Pause();
        
        _isPaused = true;
    }
}

void DirectXSound::stop()
{
    _isLooping = false;
    _isPaused = false;
    
    _soundEffectInstance->Stop();
}

void DirectXSound::setVolume(float volume)
{
    _soundEffectInstance->SetVolume(volume);
}

bool DirectXSound::isLooping()
{
    return _isLooping;
}

bool DirectXSound::isPlaying()
{
	return _soundEffectInstance->GetState() == DirectX::SoundState::PLAYING;
}

bool DirectXSound::isPaused()
{
    return _isPaused;
}
