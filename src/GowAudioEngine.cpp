//
//  GowAudioEngine.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "GowAudioEngine.hpp"

#include "SoundWrapper.hpp"
#include "Sound.hpp"
#include "AudioEngineHelper.hpp"
#include "AudioEngineHelperFactory.hpp"
#include "MathUtil.hpp"
#include "Assets.hpp"
#include "SoundDescriptor.hpp"
#include "STLUtil.hpp"
#include "AssetManager.hpp"

#include <assert.h>

#define MAX_SOUNDS_TO_PLAY_PER_FRAME 3

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

void GowAudioEngine::pause()
{
    pauseMusic();
    pauseAllSounds();
    AUDIO_ENGINE_HELPER.pause();
}

void GowAudioEngine::resume()
{
    AUDIO_ENGINE_HELPER.resume();
    resumeMusic();
    resumeAllSounds();
}

void GowAudioEngine::playSound(uint16_t soundID, float volume, bool isLooping)
{
    if (AUDIO_ENGINE_HELPER.areSoundsDisabled() ||
        _soundsToPlay.size() >= MAX_SOUNDS_TO_PLAY_PER_FRAME)
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

void GowAudioEngine::stopSound(uint16_t soundID)
{
    if (AUDIO_ENGINE_HELPER.areSoundsDisabled())
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

void GowAudioEngine::pauseSound(uint16_t soundID)
{
    if (AUDIO_ENGINE_HELPER.areSoundsDisabled())
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

void GowAudioEngine::resumeSound(uint16_t soundID)
{
    if (AUDIO_ENGINE_HELPER.areSoundsDisabled())
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

void GowAudioEngine::stopAllSounds()
{
    if (AUDIO_ENGINE_HELPER.areSoundsDisabled())
    {
        return;
    }
    
    for (std::map<uint16_t, SoundWrapper*>::iterator i = _sounds.begin(); i != _sounds.end(); ++i)
    {
        std::vector<Sound *> sounds = (*i).second->getSounds();
        for (std::vector<Sound *>::iterator i = sounds.begin(); i != sounds.end(); ++i)
        {
            _soundsToStop.push_back((*i));
        }
    }
}

void GowAudioEngine::pauseAllSounds()
{
    if (AUDIO_ENGINE_HELPER.areSoundsDisabled())
    {
        return;
    }
    
    for (std::map<uint16_t, SoundWrapper*>::iterator i = _sounds.begin(); i != _sounds.end(); ++i)
    {
        std::vector<Sound *> sounds = (*i).second->getSounds();
        for (std::vector<Sound *>::iterator i = sounds.begin(); i != sounds.end(); ++i)
        {
            if ((*i)->isPlaying())
            {
                _soundsToPause.push_back((*i));
            }
        }
    }
}

void GowAudioEngine::resumeAllSounds()
{
    if (AUDIO_ENGINE_HELPER.areSoundsDisabled())
    {
        return;
    }
    
    for (std::map<uint16_t, SoundWrapper*>::iterator i = _sounds.begin(); i != _sounds.end(); ++i)
    {
        std::vector<Sound *> sounds = (*i).second->getSounds();
        for (std::vector<Sound *>::iterator i = sounds.begin(); i != sounds.end(); ++i)
        {
            if ((*i)->isPaused())
            {
                _soundsToResume.push_back((*i));
            }
        }
    }
}

void GowAudioEngine::playMusic(float volume, bool isLooping)
{
    SoundWrapper* music = ASSETS.music();
    if (AUDIO_ENGINE_HELPER.isMusicDisabled() ||
        music == NULL)
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
    if (AUDIO_ENGINE_HELPER.isMusicDisabled() ||
        music == NULL)
    {
        return;
    }
    
    music->setVolume(CLAMP(volume, 0, 1));
}

void GowAudioEngine::stopMusic()
{
    SoundWrapper* music = ASSETS.music();
    if (AUDIO_ENGINE_HELPER.isMusicDisabled() ||
        music == NULL)
    {
        return;
    }
    
    _soundsToStop.push_back(music->soundInstance());
}

void GowAudioEngine::pauseMusic()
{
    SoundWrapper* music = ASSETS.music();
    if (AUDIO_ENGINE_HELPER.isMusicDisabled() ||
        music == NULL)
    {
        return;
    }
    
    _soundsToPause.push_back(music->soundInstance());
}

void GowAudioEngine::resumeMusic()
{
    SoundWrapper* music = ASSETS.music();
    if (AUDIO_ENGINE_HELPER.isMusicDisabled() ||
        music == NULL)
    {
        return;
    }
    
    _soundsToResume.push_back(music->soundInstance());
}

bool GowAudioEngine::isMusicPlaying()
{
    SoundWrapper* music = ASSETS.music();
    if (AUDIO_ENGINE_HELPER.isMusicDisabled() ||
        music == NULL)
    {
        return false;
    }
    
    return music->isPlaying();
}
