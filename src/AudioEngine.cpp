//
//  AudioEngine.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "AudioEngine.hpp"

#include "SoundWrapper.hpp"
#include "Sound.hpp"
#include "AudioEngine.hpp"
#include "AudioEngineFactory.hpp"
#include "MathUtil.hpp"
#include "Assets.hpp"
#include "SoundDescriptor.hpp"
#include "STLUtil.hpp"
#include "AssetsManager.hpp"

#define MAX_SOUNDS_TO_PLAY_PER_FRAME 4

void AudioEngine::pause()
{
    pauseMusic();
    pauseAllSounds();
    onPause();
}

void AudioEngine::resume()
{
    onResume();
    resumeMusic();
    resumeAllSounds();
}

void AudioEngine::render()
{
    for (Sound* s : _soundsToPlay)
    {
        s->play();
    }
    _soundsToPlay.clear();
    
    for (Sound* s : _soundsToStop)
    {
        s->stop();
    }
    _soundsToStop.clear();

    for (Sound* s : _soundsToPause)
    {
        s->pause();
    }
    _soundsToPause.clear();

    for (Sound* s : _soundsToResume)
    {
        s->resume();
    }
    _soundsToResume.clear();
}

void AudioEngine::playSound(uint16_t soundID, float volume, bool isLooping)
{
    if (_soundsDisabled || _soundsToPlay.size() >= MAX_SOUNDS_TO_PLAY_PER_FRAME)
    {
        return;
    }
    
    SoundWrapper* sw = ASSETS.sound(soundID);
    if (sw == NULL)
    {
        return;
    }
    
    Sound* s = sw->nextSoundInstance();
    s->setVolume(CLAMP(volume, 0, 1));
    s->setLooping(isLooping);
    
    _soundsToPlay.push_back(s);
}

void AudioEngine::stopSound(uint16_t soundID)
{
    if (_soundsDisabled)
    {
        return;
    }
    
    SoundWrapper* sw = ASSETS.sound(soundID);
    if (sw == NULL)
    {
        return;
    }
    
    for (uint8_t i = 0; i < sw->numInstances(); ++i)
    {
        Sound* s = sw->soundInstance(i);
        _soundsToStop.push_back(s);
    }
}

void AudioEngine::pauseSound(uint16_t soundID)
{
    if (_soundsDisabled)
    {
        return;
    }
    
    SoundWrapper* sw = ASSETS.sound(soundID);
    if (sw == NULL)
    {
        return;
    }
    
    for (uint8_t i = 0; i < sw->numInstances(); ++i)
    {
        Sound* s = sw->soundInstance(i);
        _soundsToPause.push_back(s);
    }
}

void AudioEngine::resumeSound(uint16_t soundID)
{
    if (_soundsDisabled)
    {
        return;
    }
    
    SoundWrapper* sw = ASSETS.sound(soundID);
    if (sw == NULL)
    {
        return;
    }
    
    for (uint8_t i = 0; i < sw->numInstances(); ++i)
    {
        Sound* s = sw->soundInstance(i);
        _soundsToResume.push_back(s);
    }
}

void AudioEngine::stopAllSounds()
{
    if (_soundsDisabled)
    {
        return;
    }
    
    std::map<uint16_t, SoundWrapper*>& sounds = ASSETS.sounds();
    for (auto& pair : sounds)
    {
        for (auto* s : pair.second->getSounds())
        {
            _soundsToStop.push_back(s);
        }
    }
}

void AudioEngine::pauseAllSounds()
{
    if (_soundsDisabled)
    {
        return;
    }
    
    std::map<uint16_t, SoundWrapper*>& sounds = ASSETS.sounds();
    for (auto& pair : sounds)
    {
        for (auto* s : pair.second->getSounds())
        {
            if (s->isPlaying())
            {
                _soundsToPause.push_back(s);
            }
        }
    }
}

void AudioEngine::resumeAllSounds()
{
    if (_soundsDisabled)
    {
        return;
    }
    
    std::map<uint16_t, SoundWrapper*>& sounds = ASSETS.sounds();
    for (auto& pair : sounds)
    {
        for (auto* s : pair.second->getSounds())
        {
            if (s->isPaused())
            {
                _soundsToResume.push_back(s);
            }
        }
    }
}

void AudioEngine::playMusic(float volume, bool isLooping)
{
    SoundWrapper* music = ASSETS.music();
    if (_musicDisabled || music == NULL)
    {
        return;
    }
    
    music->setVolume(CLAMP(volume, 0, 1));
    music->setLooping(isLooping);
    
    _soundsToPlay.push_back(music->soundInstance());
}

void AudioEngine::setMusicVolume(float volume)
{
    SoundWrapper* music = ASSETS.music();
    if (_musicDisabled || music == NULL)
    {
        return;
    }
    
    music->setVolume(CLAMP(volume, 0, 1));
}

void AudioEngine::stopMusic()
{
    SoundWrapper* music = ASSETS.music();
    if (_musicDisabled || music == NULL)
    {
        return;
    }
    
    _soundsToStop.push_back(music->soundInstance());
}

void AudioEngine::pauseMusic()
{
    SoundWrapper* music = ASSETS.music();
    if (_musicDisabled || music == NULL)
    {
        return;
    }
    
    _soundsToPause.push_back(music->soundInstance());
}

void AudioEngine::resumeMusic()
{
    SoundWrapper* music = ASSETS.music();
    if (_musicDisabled || music == NULL)
    {
        return;
    }
    
    _soundsToResume.push_back(music->soundInstance());
}

bool AudioEngine::isMusicPlaying()
{
    SoundWrapper* music = ASSETS.music();
    if (isMusicDisabled() ||
        music == NULL)
    {
        return false;
    }
    
    return music->isPlaying();
}

bool AudioEngine::isMusicDisabled()
{
    return _musicDisabled;
}

void AudioEngine::setMusicDisabled(bool value)
{
    _musicDisabled = value;
}

bool AudioEngine::areSoundsDisabled()
{
    return _soundsDisabled;
}

void AudioEngine::setSoundsDisabled(bool value)
{
    _soundsDisabled = value;
}

AudioEngine::AudioEngine() :
_musicDisabled(false),
_soundsDisabled(false)
{
    // Empty
}
