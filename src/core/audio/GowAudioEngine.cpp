//
//  GowAudioEngine.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#define MAX_SOUNDS_TO_PLAY_PER_FRAME 4

void GowAudioEngine::pause()
{
    pauseMusic();
    pauseAllSounds();
    onPause();
}

void GowAudioEngine::resume()
{
    onResume();
    resumeMusic();
    resumeAllSounds();
}

void GowAudioEngine::render()
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

void GowAudioEngine::playSound(uint16_t soundID, float volume, bool isLooping)
{
    if (_soundsDisabled || _soundsToPlay.size() >= MAX_SOUNDS_TO_PLAY_PER_FRAME)
    {
        return;
    }
    
    SoundWrapper* sw = ASSETS.sound(soundID);
    if (sw == nullptr)
    {
        return;
    }
    
    Sound* s = sw->nextSoundInstance();
    s->setVolume(CLAMP(volume, 0, 1));
    s->setLooping(isLooping);
    
    _soundsToPlay.push_back(s);
}

void GowAudioEngine::stopSound(uint16_t soundID)
{
    if (_soundsDisabled)
    {
        return;
    }
    
    SoundWrapper* sw = ASSETS.sound(soundID);
    if (sw == nullptr)
    {
        return;
    }
    
    for (uint8_t i = 0; i < sw->numInstances(); ++i)
    {
        Sound* s = sw->soundInstance(i);
        _soundsToStop.push_back(s);
    }
}

void GowAudioEngine::pauseSound(uint16_t soundID)
{
    if (_soundsDisabled)
    {
        return;
    }
    
    SoundWrapper* sw = ASSETS.sound(soundID);
    if (sw == nullptr)
    {
        return;
    }
    
    for (uint8_t i = 0; i < sw->numInstances(); ++i)
    {
        Sound* s = sw->soundInstance(i);
        _soundsToPause.push_back(s);
    }
}

void GowAudioEngine::resumeSound(uint16_t soundID)
{
    if (_soundsDisabled)
    {
        return;
    }
    
    SoundWrapper* sw = ASSETS.sound(soundID);
    if (sw == nullptr)
    {
        return;
    }
    
    for (uint8_t i = 0; i < sw->numInstances(); ++i)
    {
        Sound* s = sw->soundInstance(i);
        _soundsToResume.push_back(s);
    }
}

void GowAudioEngine::stopAllSounds()
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

void GowAudioEngine::pauseAllSounds()
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

void GowAudioEngine::resumeAllSounds()
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

void GowAudioEngine::playMusic(float volume, bool isLooping)
{
    SoundWrapper* music = ASSETS.music();
    if (_musicDisabled || music == nullptr)
    {
        return;
    }
    
    music->setVolume(CLAMP(volume, 0, 1));
    music->setLooping(isLooping);
    
    _soundsToPlay.push_back(music->soundInstance());
}

void GowAudioEngine::setMusicVolume(float volume)
{
    SoundWrapper* music = ASSETS.music();
    if (_musicDisabled || music == nullptr)
    {
        return;
    }
    
    music->setVolume(CLAMP(volume, 0, 1));
}

void GowAudioEngine::stopMusic()
{
    SoundWrapper* music = ASSETS.music();
    if (_musicDisabled || music == nullptr)
    {
        return;
    }
    
    _soundsToStop.push_back(music->soundInstance());
}

void GowAudioEngine::pauseMusic()
{
    SoundWrapper* music = ASSETS.music();
    if (_musicDisabled || music == nullptr)
    {
        return;
    }
    
    _soundsToPause.push_back(music->soundInstance());
}

void GowAudioEngine::resumeMusic()
{
    SoundWrapper* music = ASSETS.music();
    if (_musicDisabled || music == nullptr)
    {
        return;
    }
    
    _soundsToResume.push_back(music->soundInstance());
}

bool GowAudioEngine::isMusicPlaying()
{
    SoundWrapper* music = ASSETS.music();
    if (isMusicDisabled() ||
        music == nullptr)
    {
        return false;
    }
    
    return music->isPlaying();
}

bool GowAudioEngine::isMusicDisabled()
{
    return _musicDisabled;
}

void GowAudioEngine::setMusicDisabled(bool value)
{
    _musicDisabled = value;
}

bool GowAudioEngine::areSoundsDisabled()
{
    return _soundsDisabled;
}

void GowAudioEngine::setSoundsDisabled(bool value)
{
    _soundsDisabled = value;
}

GowAudioEngine::GowAudioEngine() :
_musicDisabled(false),
_soundsDisabled(false)
{
    // Empty
}
