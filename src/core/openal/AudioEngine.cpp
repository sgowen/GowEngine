//
//  AudioEngine.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/21/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
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
    for (uint32_t s : _soundsToPlay)
    {
        OAL.play(s);
    }
    _soundsToPlay.clear();
    
    for (uint32_t s : _soundsToStop)
    {
        OAL.stop(s);
    }
    _soundsToStop.clear();

    for (uint32_t s : _soundsToPause)
    {
        OAL.pause(s);
    }
    _soundsToPause.clear();

    for (uint32_t s : _soundsToResume)
    {
        OAL.resume(s);
    }
    _soundsToResume.clear();
}

void AudioEngine::playSound(std::string soundID, float volume, bool isLooping)
{
    bool audioDisabled = ENGINE_CFG.audioDisabled();
    uint8_t maxNumSoundsToPlayPerFrame = ENGINE_CFG.maxNumSoundsToPlayPerFrame();
    if (audioDisabled || _soundsToPlay.size() >= maxNumSoundsToPlayPerFrame)
    {
        return;
    }
    
    Sound& s = ASSETS_MGR.sound(soundID);
    uint32_t alHandle = s.nextSource();
    
    OAL.setVolume(alHandle, CLAMP(volume, 0, 1));
    OAL.setLooping(alHandle, isLooping);
    
    _soundsToPlay.push_back(alHandle);
}

void AudioEngine::stopSound(std::string soundID)
{
    bool audioDisabled = ENGINE_CFG.audioDisabled();
    if (audioDisabled)
    {
        return;
    }
    
    Sound& s = ASSETS_MGR.sound(soundID);
    
    for (uint8_t i = 0; i < s._desc._numInstances; ++i)
    {
        _soundsToStop.push_back(s.nextSource());
    }
}

void AudioEngine::pauseSound(std::string soundID)
{
    bool audioDisabled = ENGINE_CFG.audioDisabled();
    if (audioDisabled)
    {
        return;
    }
    
    Sound& s = ASSETS_MGR.sound(soundID);
    
    for (uint8_t i = 0; i < s._desc._numInstances; ++i)
    {
        _soundsToPause.push_back(s.nextSource());
    }
}

void AudioEngine::resumeSound(std::string soundID)
{
    bool audioDisabled = ENGINE_CFG.audioDisabled();
    if (audioDisabled)
    {
        return;
    }
    
    Sound& s = ASSETS_MGR.sound(soundID);
    
    for (uint8_t i = 0; i < s._desc._numInstances; ++i)
    {
        _soundsToResume.push_back(s.nextSource());
    }
}

void AudioEngine::stopAllSounds()
{
    bool audioDisabled = ENGINE_CFG.audioDisabled();
    if (audioDisabled)
    {
        return;
    }
    
    std::map<std::string, Sound>& sounds = ASSETS_MGR.sounds();
    for (auto& pair : sounds)
    {
        Sound& s = pair.second;
        for (uint8_t i = 0; i < s._desc._numInstances; ++i)
        {
            _soundsToStop.push_back(s.nextSource());
        }
    }
}

void AudioEngine::pauseAllSounds()
{
    bool audioDisabled = ENGINE_CFG.audioDisabled();
    if (audioDisabled)
    {
        return;
    }
    
    std::map<std::string, Sound>& sounds = ASSETS_MGR.sounds();
    for (auto& pair : sounds)
    {
        Sound& s = pair.second;
        for (uint8_t i = 0; i < s._desc._numInstances; ++i)
        {
            _soundsToPause.push_back(s.nextSource());
        }
    }
}

void AudioEngine::resumeAllSounds()
{
    bool audioDisabled = ENGINE_CFG.audioDisabled();
    if (audioDisabled)
    {
        return;
    }
    
    std::map<std::string, Sound>& sounds = ASSETS_MGR.sounds();
    for (auto& pair : sounds)
    {
        Sound& s = pair.second;
        for (uint8_t i = 0; i < s._desc._numInstances; ++i)
        {
            _soundsToResume.push_back(s.nextSource());
        }
    }
}

AudioEngine::AudioEngine()
{
    OAL.openDeviceAndCreateContext();
}

AudioEngine::~AudioEngine()
{
    OAL.destroyContextAndCloseDevice();
}
