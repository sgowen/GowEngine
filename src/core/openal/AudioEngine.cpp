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

void AudioEngine::playSoundsForWorld(World& w)
{
    if (isPaused() || w.getNumMovesProcessed() == 0)
    {
        return;
    }
    
    uint32_t currentMoveIndex = w.getNumMovesProcessed() - 1;
    SoundFrameState& sfs = soundFrameStateAtMoveIndex(currentMoveIndex);
    if (sfs._frame < currentMoveIndex)
    {
        sfs._frame = currentMoveIndex;
        sfs._entitySoundStates.clear();
    }
    
    for (Entity* e : w.getPlayers())
    {
        playSoundForEntityIfNecessary(*e, currentMoveIndex);
    }
    
    for (Entity* e : w.getDynamicEntities())
    {
        playSoundForEntityIfNecessary(*e, currentMoveIndex);
    }
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

void AudioEngine::playMusic(std::string soundID, float volume, bool isLooping)
{
    if (ENGINE_CFG.audioDisabled() || ENGINE_CFG.musicDisabled())
    {
        return;
    }
    
    playSound(soundID, 0, volume, isLooping);
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

AudioEngine::SoundFrameState& AudioEngine::soundFrameStateAtMoveIndex(uint32_t moveIndex)
{
    uint32_t index = moveIndex % 360;
    
    return _soundFrameStates[index];
}

void AudioEngine::playSoundForEntityIfNecessary(Entity& e, uint32_t moveIndex)
{
    // TODO, support more than 1 sound per entity per frame
    
    if (e.isExiled())
    {
        return;
    }
    
    std::string textureMapping = e.controllerRender()->getTextureMapping();
    std::vector<SoundMapping>* soundMappings = e.controllerRender()->getSoundMapping();
    if (soundMappings == nullptr)
    {
        return;
    }
    
    uint16_t soundStateTime = e.stateTime();
    Animation* animation = ASSETS_MGR.animation(textureMapping);
    if (animation != nullptr &&
        animation->isLooping() &&
        // TODO, this is just a crude hack to make sure
        // we don't hear the drill stomp sound effect more than once
        animation->cycleTimeBeforeFirstLoopingFrame() == 0)
    {
        soundStateTime = e.stateTime() % animation->cycleTime();
    }
    
    SoundMapping* soundMapping = nullptr;
    for (int i = 0; i < soundMappings->size(); ++i)
    {
        SoundMapping& sm = soundMappings->at(i);
        if (soundStateTime >= sm._stateTime)
        {
            soundMapping = &sm;
        }
    }
    
    if (soundMapping == nullptr)
    {
        return;
    }
    
    std::string soundID = soundMapping->_soundID;
    uint16_t stateTimeToCheck = soundStateTime - soundMapping->_stateTime;
    
    uint32_t moveIndexToCheck = 0;
    uint32_t numFramesToSkip = 0;
    if (moveIndex >= stateTimeToCheck)
    {
        moveIndexToCheck = moveIndex - stateTimeToCheck;
        numFramesToSkip = soundStateTime - soundMapping->_stateTime;
    }
    
    std::string soundStateMapping = STRING_FORMAT("%d+%s+%d+%d", e.getID(), soundID.c_str(), e.state()._state, soundMapping->_stateTime);
    
    SoundFrameState& sfs = soundFrameStateAtMoveIndex(moveIndexToCheck);
    
    std::map<std::string, uint32_t>* entitySoundState = nullptr;
    
    const auto& entitySoundStateQuery = sfs._entitySoundStates.find(e.getID());
    if (entitySoundStateQuery == sfs._entitySoundStates.end())
    {
        sfs._entitySoundStates.emplace(e.getID(), std::map<std::string, uint32_t>());
        sfs._frame = moveIndexToCheck;
        entitySoundState = &sfs._entitySoundStates[e.getID()];
    }
    else
    {
        entitySoundState = &entitySoundStateQuery->second;
    }
    
    const auto& soundHandleQuery = entitySoundState->find(soundStateMapping);
    if (soundHandleQuery == entitySoundState->end())
    {
        if (ENGINE_CFG.logOpenAL())
        {
            LOG("Playing soundStateMapping: %s", soundStateMapping.c_str());
        }
        uint32_t soundHandle = AUDIO_ENGINE.playSound(soundID, numFramesToSkip);
        
        if (ENGINE_CFG.logOpenAL())
        {
            LOG("Played sound %d seeked ahead %d frames", soundHandle, numFramesToSkip);
        }
        
        entitySoundState->emplace(soundStateMapping, soundHandle);
    }
}

AudioEngine::AudioEngine() : _isPaused(false)
{
    OAL.openDeviceAndCreateContext();
}

AudioEngine::~AudioEngine()
{
    OAL.destroyContextAndCloseDevice();
}
