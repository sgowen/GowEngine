//
//  GowAudioEngine.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

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

void GowAudioEngine::playSound(std::string soundID, float volume, bool isLooping)
{
    bool soundsDisabled = ENGINE_CFG.soundsDisabled();
    uint8_t maxNumSoundsToPlayPerFrame = ENGINE_CFG.maxNumSoundsToPlayPerFrame();
    if (soundsDisabled || _soundsToPlay.size() >= maxNumSoundsToPlayPerFrame)
    {
        return;
    }
    
    SoundWrapper* sw = ASSETS_MGR.sound(soundID);
    if (sw == nullptr)
    {
        return;
    }
    
    Sound* s = sw->nextSoundInstance();
    s->setVolume(CLAMP(volume, 0, 1));
    s->setLooping(isLooping);
    
    _soundsToPlay.push_back(s);
}

void GowAudioEngine::stopSound(std::string soundID)
{
    bool soundsDisabled = ENGINE_CFG.soundsDisabled();
    if (soundsDisabled)
    {
        return;
    }
    
    SoundWrapper* sw = ASSETS_MGR.sound(soundID);
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

void GowAudioEngine::pauseSound(std::string soundID)
{
    bool soundsDisabled = ENGINE_CFG.soundsDisabled();
    if (soundsDisabled)
    {
        return;
    }
    
    SoundWrapper* sw = ASSETS_MGR.sound(soundID);
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

void GowAudioEngine::resumeSound(std::string soundID)
{
    bool soundsDisabled = ENGINE_CFG.soundsDisabled();
    if (soundsDisabled)
    {
        return;
    }
    
    SoundWrapper* sw = ASSETS_MGR.sound(soundID);
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
    bool soundsDisabled = ENGINE_CFG.soundsDisabled();
    if (soundsDisabled)
    {
        return;
    }
    
    std::map<std::string, SoundWrapper*>& sounds = ASSETS_MGR.sounds();
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
    bool soundsDisabled = ENGINE_CFG.soundsDisabled();
    if (soundsDisabled)
    {
        return;
    }
    
    std::map<std::string, SoundWrapper*>& sounds = ASSETS_MGR.sounds();
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
    bool soundsDisabled = ENGINE_CFG.soundsDisabled();
    if (soundsDisabled)
    {
        return;
    }
    
    std::map<std::string, SoundWrapper*>& sounds = ASSETS_MGR.sounds();
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
    bool musicDisabled = ENGINE_CFG.musicDisabled();
    SoundWrapper* music = ASSETS_MGR.music();
    if (musicDisabled || music == nullptr)
    {
        return;
    }
    
    music->setVolume(CLAMP(volume, 0, 1));
    music->setLooping(isLooping);
    
    _soundsToPlay.push_back(music->soundInstance());
}

void GowAudioEngine::setMusicVolume(float volume)
{
    bool musicDisabled = ENGINE_CFG.musicDisabled();
    SoundWrapper* music = ASSETS_MGR.music();
    if (musicDisabled || music == nullptr)
    {
        return;
    }
    
    music->setVolume(CLAMP(volume, 0, 1));
}

void GowAudioEngine::stopMusic()
{
    bool musicDisabled = ENGINE_CFG.musicDisabled();
    SoundWrapper* music = ASSETS_MGR.music();
    if (musicDisabled || music == nullptr)
    {
        return;
    }
    
    _soundsToStop.push_back(music->soundInstance());
}

void GowAudioEngine::pauseMusic()
{
    bool musicDisabled = ENGINE_CFG.musicDisabled();
    SoundWrapper* music = ASSETS_MGR.music();
    if (musicDisabled || music == nullptr)
    {
        return;
    }
    
    _soundsToPause.push_back(music->soundInstance());
}

void GowAudioEngine::resumeMusic()
{
    bool musicDisabled = ENGINE_CFG.musicDisabled();
    SoundWrapper* music = ASSETS_MGR.music();
    if (musicDisabled || music == nullptr)
    {
        return;
    }
    
    _soundsToResume.push_back(music->soundInstance());
}

bool GowAudioEngine::isMusicPlaying()
{
    bool musicDisabled = ENGINE_CFG.musicDisabled();
    SoundWrapper* music = ASSETS_MGR.music();
    if (musicDisabled || music == nullptr)
    {
        return false;
    }
    
    return music->isPlaying();
}

GowAudioEngine::GowAudioEngine()
{
    // Empty
}

GowAudioEngine::~GowAudioEngine()
{
    // Empty
}
