//
//  WindowsSound.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/10/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "WindowsSound.hpp"

WindowsSound::WindowsSound(DirectX::SoundEffect& sound, float volume) : Sound()
{
    _soundEffectInstance = sound.CreateInstance();
}

WindowsSound::~WindowsSound()
{
    stop();
    
    _soundEffectInstance.reset();
}

void WindowsSound::play()
{
	_soundEffectInstance->Stop();
    _soundEffectInstance->Play(_isLooping);
}

void WindowsSound::resume()
{
    if (isPaused())
    {
        _soundEffectInstance->Resume();
    }
}

void WindowsSound::pause()
{
    if (isPlaying())
    {
        _soundEffectInstance->Pause();
    }
}

void WindowsSound::stop()
{    
    _soundEffectInstance->Stop();
}

void WindowsSound::setVolume(float volume)
{
    _soundEffectInstance->SetVolume(volume);
}

bool WindowsSound::isLooping()
{
    return _soundEffectInstance->IsLooped();
}

bool WindowsSound::isPlaying()
{
	return _soundEffectInstance->GetState() == DirectX::SoundState::PLAYING;
}

bool WindowsSound::isPaused()
{
    return !isPlaying();
}
