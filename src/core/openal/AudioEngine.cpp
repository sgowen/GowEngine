//
//  AudioEngine.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/21/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <stdio.h>
#include <errno.h>
#include <string.h>

AudioEngine* AudioEngine::s_instance = nullptr;

void AudioEngine::create()
{
    assert(s_instance == nullptr);
    
    s_instance = new AudioEngine();
}

AudioEngine& AudioEngine::getInstance()
{
    assert(s_instance != nullptr);
    
    return *s_instance;
}

void AudioEngine::destroy()
{
    assert(s_instance != nullptr);
    
    delete s_instance;
    s_instance = nullptr;
}

void AudioEngine::pause()
{
    pauseAllSounds();
}

void AudioEngine::resume()
{
    resumeAllSounds();
}

void AudioEngine::render()
{
    for (auto i = _soundsToPlay.begin(); i != _soundsToPlay.end(); ++i)
    {
        uint32_t alHandle = i->first;
        SoundCommand& sc = i->second;
        
        OAL.setVolume(alHandle, sc._volume);
        OAL.setLooping(alHandle, sc._isLooping);
        
        if (sc._numFramesSeekedAhead > 0)
        {
            OAL.seekAhead(alHandle, sc._numFramesSeekedAhead);
        }
        
        OAL.play(alHandle);
    }
    _soundsToPlay.clear();
    
    for (uint32_t alHandle : _soundsToStop)
    {
        OAL.stop(alHandle);
    }
    _soundsToStop.clear();

    for (uint32_t alHandle : _soundsToPause)
    {
        OAL.pause(alHandle);
    }
    _soundsToPause.clear();

    for (uint32_t alHandle : _soundsToResume)
    {
        OAL.resume(alHandle);
    }
    _soundsToResume.clear();
}

uint32_t AudioEngine::playSound(std::string soundID, uint32_t numFramesSeekedAhead, float volume, bool isLooping)
{
    if (ENGINE_CFG.audioDisabled())
    {
        return 0;
    }
    
    Sound& s = ASSETS_MGR.sound(soundID);
    uint32_t alHandle = s.nextSource();
    
    float realVolume = ENGINE_CFG.volume() * volume;
    
    _soundsToPlay.emplace(alHandle, SoundCommand(numFramesSeekedAhead, CLAMP(realVolume, 0, 1), isLooping));
    
    return alHandle;
}

void AudioEngine::stopSound(std::string soundID)
{
    if (ENGINE_CFG.audioDisabled())
    {
        return;
    }
    
    Sound& s = ASSETS_MGR.sound(soundID);
    
    for (uint8_t i = 0; i < s._desc._numInstances; ++i)
    {
        uint32_t alHandle = s.nextSource();
        _soundsToStop.push_back(alHandle);
    }
}

void AudioEngine::pauseSound(std::string soundID)
{
    if (ENGINE_CFG.audioDisabled())
    {
        return;
    }
    
    Sound& s = ASSETS_MGR.sound(soundID);
    
    for (uint8_t i = 0; i < s._desc._numInstances; ++i)
    {
        uint32_t alHandle = s.nextSource();
        _soundsToPause.push_back(alHandle);
    }
}

void AudioEngine::resumeSound(std::string soundID)
{
    if (ENGINE_CFG.audioDisabled())
    {
        return;
    }
    
    Sound& s = ASSETS_MGR.sound(soundID);
    
    for (uint8_t i = 0; i < s._desc._numInstances; ++i)
    {
        uint32_t alHandle = s.nextSource();
        _soundsToResume.push_back(alHandle);
    }
}

void AudioEngine::stopAllSounds()
{
    if (ENGINE_CFG.audioDisabled())
    {
        return;
    }
    
    if (!ASSETS_MGR.areSoundsLoaded())
    {
        return;
    }
    
    std::map<std::string, Sound>& sounds = ASSETS_MGR.sounds();
    for (auto& pair : sounds)
    {
        Sound& s = pair.second;
        for (uint8_t i = 0; i < s._desc._numInstances; ++i)
        {
            uint32_t alHandle = s.nextSource();
            _soundsToStop.push_back(alHandle);
        }
    }
}

void AudioEngine::pauseAllSounds()
{
    // TODO, should be able to pause all sounds and leave music running
    
    if (_isPaused)
    {
        return;
    }
    
    if (ENGINE_CFG.audioDisabled())
    {
        return;
    }
    
    if (!ASSETS_MGR.areSoundsLoaded())
    {
        return;
    }
    
    std::map<std::string, Sound>& sounds = ASSETS_MGR.sounds();
    for (auto& pair : sounds)
    {
        Sound& s = pair.second;
        for (uint8_t i = 0; i < s._desc._numInstances; ++i)
        {
            uint32_t alHandle = s.nextSource();
            _soundsToPause.push_back(alHandle);
        }
    }
    
    _isPaused = true;
}

void AudioEngine::resumeAllSounds()
{
    // TODO, should be able to resume all sounds except music
    
    if (!_isPaused)
    {
        return;
    }
    
    if (ENGINE_CFG.audioDisabled())
    {
        return;
    }
    
    if (!ASSETS_MGR.areSoundsLoaded())
    {
        return;
    }
    
    std::map<std::string, Sound>& sounds = ASSETS_MGR.sounds();
    for (auto& pair : sounds)
    {
        Sound& s = pair.second;
        for (uint8_t i = 0; i < s._desc._numInstances; ++i)
        {
            uint32_t alHandle = s.nextSource();
            _soundsToResume.push_back(alHandle);
        }
    }
    
    _isPaused = false;
}

bool AudioEngine::isPaused()
{
    return _isPaused;
}

AudioEngine::AudioEngine() : _isPaused(false)
{
    OAL.openDeviceAndCreateContext();
}

AudioEngine::~AudioEngine()
{
    OAL.destroyContextAndCloseDevice();
}
