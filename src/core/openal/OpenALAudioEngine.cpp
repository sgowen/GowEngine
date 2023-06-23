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

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

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
        s->play();
    }
    _soundsToPlay.clear();
    
    for (OpenALSound* s : _soundsToStop)
    {
        s->stop();
    }
    _soundsToStop.clear();

    for (OpenALSound* s : _soundsToPause)
    {
        s->pause();
    }
    _soundsToPause.clear();

    for (OpenALSound* s : _soundsToResume)
    {
        s->resume();
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
    s->setVolume(CLAMP(volume, 0, 1));
    s->setLooping(isLooping);
    
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
        for (auto* s : pair.second->getSounds())
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
        for (auto* s : pair.second->getSounds())
        {
            if (s->isPlaying())
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
        for (auto* s : pair.second->getSounds())
        {
            if (s->isPaused())
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
    
    music->setVolume(CLAMP(volume, 0, 1));
    music->setLooping(isLooping);
    
    _soundsToPlay.push_back(music->soundInstance());
}

void OpenALAudioEngine::setMusicVolume(float volume)
{
    bool musicDisabled = ENGINE_CFG.musicDisabled();
    OpenALSoundWrapper* music = ASSETS_MGR.music();
    if (musicDisabled || music == nullptr)
    {
        return;
    }
    
    music->setVolume(CLAMP(volume, 0, 1));
}

void OpenALAudioEngine::stopMusic()
{
    bool musicDisabled = ENGINE_CFG.musicDisabled();
    OpenALSoundWrapper* music = ASSETS_MGR.music();
    if (musicDisabled || music == nullptr)
    {
        return;
    }
    
    _soundsToStop.push_back(music->soundInstance());
}

void OpenALAudioEngine::pauseMusic()
{
    bool musicDisabled = ENGINE_CFG.musicDisabled();
    OpenALSoundWrapper* music = ASSETS_MGR.music();
    if (musicDisabled || music == nullptr)
    {
        return;
    }
    
    _soundsToPause.push_back(music->soundInstance());
}

void OpenALAudioEngine::resumeMusic()
{
    bool musicDisabled = ENGINE_CFG.musicDisabled();
    OpenALSoundWrapper* music = ASSETS_MGR.music();
    if (musicDisabled || music == nullptr)
    {
        return;
    }
    
    _soundsToResume.push_back(music->soundInstance());
}

bool OpenALAudioEngine::isMusicPlaying()
{
    bool musicDisabled = ENGINE_CFG.musicDisabled();
    OpenALSoundWrapper* music = ASSETS_MGR.music();
    if (musicDisabled || music == nullptr)
    {
        return false;
    }
    
    return music->isPlaying();
}

OpenALAudioEngine::OpenALAudioEngine()
{
    const ALCchar *name;
    ALCdevice *device;
    ALCcontext *ctx;

    device = alcOpenDevice(nullptr);
    
    if (!device)
    {
        LOG("Could not open a device!");
        assert(false);
    }

    ctx = alcCreateContext(device, nullptr);
    if (ctx == nullptr || alcMakeContextCurrent(ctx) == ALC_FALSE)
    {
        if (ctx != nullptr)
        {
            alcDestroyContext(ctx);
        }
        alcCloseDevice(device);
        LOG("Could not set a context!");
        assert(false);
    }

    name = nullptr;
    if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
    {
        name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
    }
    
    if (!name || alcGetError(device) != AL_NO_ERROR)
    {
        name = alcGetString(device, ALC_DEVICE_SPECIFIER);
    }
    
    LOG("Opened \"%s\"\n", name);
}

OpenALAudioEngine::~OpenALAudioEngine()
{
    ALCdevice *device;
    ALCcontext *ctx;

    ctx = alcGetCurrentContext();
    if (ctx != nullptr)
    {
        device = alcGetContextsDevice(ctx);

        alcMakeContextCurrent(nullptr);
        alcDestroyContext(ctx);
        alcCloseDevice(device);
    }
}
