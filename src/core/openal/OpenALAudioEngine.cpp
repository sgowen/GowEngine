//
//  OpenALAudioEngine.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/21/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <stdio.h>
#include <errno.h>
#include <string.h>

OpenALSoundWrapper* OpenALAudioEngine::loadSound(std::string filePath, uint8_t numInstances)
{
    return new OpenALSoundWrapper(filePath, numInstances);
}

OpenALSoundWrapper* OpenALAudioEngine::loadMusic(std::string filePath)
{
    return loadSound(filePath);
}

void OpenALAudioEngine::pause()
{
    pauseMusic();
    pauseAllSounds();
}

void OpenALAudioEngine::resume()
{
    resumeMusic();
    resumeAllSounds();
}

void OpenALAudioEngine::render()
{
    for (OpenALSound* s : _soundsToPlay)
    {
        OAL.play(*s);
    }
    _soundsToPlay.clear();
    
    for (OpenALSound* s : _soundsToStop)
    {
        OAL.stop(*s);
    }
    _soundsToStop.clear();

    for (OpenALSound* s : _soundsToPause)
    {
        OAL.pause(*s);
    }
    _soundsToPause.clear();

    for (OpenALSound* s : _soundsToResume)
    {
        OAL.resume(*s);
    }
    _soundsToResume.clear();
}

void OpenALAudioEngine::playSound(std::string soundID, float volume, bool isLooping)
{
    bool soundsDisabled = ENGINE_CFG.soundsDisabled();
    uint8_t maxNumSoundsToPlayPerFrame = ENGINE_CFG.maxNumSoundsToPlayPerFrame();
    if (soundsDisabled || _soundsToPlay.size() >= maxNumSoundsToPlayPerFrame)
    {
        return;
    }
    
    OpenALSoundWrapper* sw = ASSETS_MGR.sound(soundID);
    if (sw == nullptr)
    {
        return;
    }
    
    OpenALSound* s = sw->nextSoundInstance();
    OAL.setVolume(*s, CLAMP(volume, 0, 1));
    OAL.setLooping(*s, isLooping);
    
    _soundsToPlay.push_back(s);
}

void OpenALAudioEngine::stopSound(std::string soundID)
{
    bool soundsDisabled = ENGINE_CFG.soundsDisabled();
    if (soundsDisabled)
    {
        return;
    }
    
    OpenALSoundWrapper* sw = ASSETS_MGR.sound(soundID);
    if (sw == nullptr)
    {
        return;
    }
    
    for (uint8_t i = 0; i < sw->numInstances(); ++i)
    {
        OpenALSound* s = sw->soundInstance(i);
        _soundsToStop.push_back(s);
    }
}

void OpenALAudioEngine::pauseSound(std::string soundID)
{
    bool soundsDisabled = ENGINE_CFG.soundsDisabled();
    if (soundsDisabled)
    {
        return;
    }
    
    OpenALSoundWrapper* sw = ASSETS_MGR.sound(soundID);
    if (sw == nullptr)
    {
        return;
    }
    
    for (uint8_t i = 0; i < sw->numInstances(); ++i)
    {
        OpenALSound* s = sw->soundInstance(i);
        _soundsToPause.push_back(s);
    }
}

void OpenALAudioEngine::resumeSound(std::string soundID)
{
    bool soundsDisabled = ENGINE_CFG.soundsDisabled();
    if (soundsDisabled)
    {
        return;
    }
    
    OpenALSoundWrapper* sw = ASSETS_MGR.sound(soundID);
    if (sw == nullptr)
    {
        return;
    }
    
    for (uint8_t i = 0; i < sw->numInstances(); ++i)
    {
        OpenALSound* s = sw->soundInstance(i);
        _soundsToResume.push_back(s);
    }
}

void OpenALAudioEngine::stopAllSounds()
{
    bool soundsDisabled = ENGINE_CFG.soundsDisabled();
    if (soundsDisabled)
    {
        return;
    }
    
    std::map<std::string, OpenALSoundWrapper*>& sounds = ASSETS_MGR.sounds();
    for (auto& pair : sounds)
    {
        for (auto s : pair.second->getSounds())
        {
            _soundsToStop.push_back(s);
        }
    }
}

void OpenALAudioEngine::pauseAllSounds()
{
    bool soundsDisabled = ENGINE_CFG.soundsDisabled();
    if (soundsDisabled)
    {
        return;
    }
    
    std::map<std::string, OpenALSoundWrapper*>& sounds = ASSETS_MGR.sounds();
    for (auto& pair : sounds)
    {
        for (auto s : pair.second->getSounds())
        {
            if (OAL.isPlaying(*s))
            {
                _soundsToPause.push_back(s);
            }
        }
    }
}

void OpenALAudioEngine::resumeAllSounds()
{
    bool soundsDisabled = ENGINE_CFG.soundsDisabled();
    if (soundsDisabled)
    {
        return;
    }
    
    std::map<std::string, OpenALSoundWrapper*>& sounds = ASSETS_MGR.sounds();
    for (auto& pair : sounds)
    {
        for (auto s : pair.second->getSounds())
        {
            if (OAL.isPaused(*s))
            {
                _soundsToResume.push_back(s);
            }
        }
    }
}

void OpenALAudioEngine::playMusic(float volume, bool isLooping)
{
    bool musicDisabled = ENGINE_CFG.musicDisabled();
    OpenALSoundWrapper* music = ASSETS_MGR.music();
    if (musicDisabled || music == nullptr)
    {
        return;
    }
    
    OpenALSound* s = music->soundInstance();
    OAL.setVolume(*s, CLAMP(volume, 0, 1));
    OAL.setLooping(*s, isLooping);
    
    _soundsToPlay.push_back(s);
}

void OpenALAudioEngine::setMusicVolume(float volume)
{
    bool musicDisabled = ENGINE_CFG.musicDisabled();
    OpenALSoundWrapper* music = ASSETS_MGR.music();
    if (musicDisabled || music == nullptr)
    {
        return;
    }
    
    OpenALSound* s = music->soundInstance();
    OAL.setVolume(*s, CLAMP(volume, 0, 1));
}

void OpenALAudioEngine::stopMusic()
{
    bool musicDisabled = ENGINE_CFG.musicDisabled();
    OpenALSoundWrapper* music = ASSETS_MGR.music();
    if (musicDisabled || music == nullptr)
    {
        return;
    }
    
    OpenALSound* s = music->soundInstance();
    _soundsToStop.push_back(s);
}

void OpenALAudioEngine::pauseMusic()
{
    bool musicDisabled = ENGINE_CFG.musicDisabled();
    OpenALSoundWrapper* music = ASSETS_MGR.music();
    if (musicDisabled || music == nullptr)
    {
        return;
    }
    
    OpenALSound* s = music->soundInstance();
    _soundsToPause.push_back(s);
}

void OpenALAudioEngine::resumeMusic()
{
    bool musicDisabled = ENGINE_CFG.musicDisabled();
    OpenALSoundWrapper* music = ASSETS_MGR.music();
    if (musicDisabled || music == nullptr)
    {
        return;
    }
    
    OpenALSound* s = music->soundInstance();
    _soundsToResume.push_back(s);
}

bool OpenALAudioEngine::isMusicPlaying()
{
    bool musicDisabled = ENGINE_CFG.musicDisabled();
    OpenALSoundWrapper* music = ASSETS_MGR.music();
    if (musicDisabled || music == nullptr)
    {
        return false;
    }
    
    OpenALSound* s = music->soundInstance();
    
    return OAL.isPlaying(*s);
}

OpenALAudioEngine::OpenALAudioEngine()
{
    assert(OAL.openDeviceAndCreateContext());
}

OpenALAudioEngine::~OpenALAudioEngine()
{
    assert(OAL.destroyContextAndCloseDevice());
}
