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
#include "GowUtil.hpp"

#include <assert.h>

#define MAX_SOUNDS_TO_PLAY_PER_FRAME 3

GowAudioEngine& GowAudioEngine::getInstance()
{
    static GowAudioEngine ret = GowAudioEngine();
    return ret;
}

void GowAudioEngine::createDeviceDependentResources()
{
    _audioEngineHelper = AudioEngineHelperFactory::create();
    _state = GAES_DEFAULT;
    
    std::vector<SoundDescriptor>& soundDescriptors = ASSETS.getSoundDescriptors();
    for (SoundDescriptor& sd : soundDescriptors)
    {
        uint16_t soundID = sd._soundID;
        if (soundID == 1337)
        {
            loadMusic(sd._filePath.c_str());
        }
        else
        {
            loadSound(soundID, sd._filePath.c_str(), sd._numInstances);
        }
    }
}

void GowAudioEngine::releaseDeviceDependentResources()
{
    GowUtil::cleanUpMapOfPointers(_sounds);
    
    _soundsToPlay.clear();
    _soundsToStop.clear();
    _soundsToPause.clear();
    _soundsToResume.clear();
    _soundsVolumes.clear();
    _soundsLooping.clear();
    
    if (_music != NULL)
    {
        _music->getSoundInstance()->stop();
        delete _music;
        _music = NULL;
    }
    
    _musicStates.clear();
    
    AudioEngineHelperFactory::destroy(_audioEngineHelper);
}

void GowAudioEngine::render()
{
    if (_state == GAES_RESUME)
    {
        _audioEngineHelper->resume();
        _state = GAES_DEFAULT;
    }
    
    {
        int len = static_cast<int>(_soundsToPlay.size());
        for (int i = 0; i < len; ++i)
        {
            Sound* sound = _soundsToPlay[i];
            float volume = _soundsVolumes[i];
            bool isLooping = _soundsLooping[i];
            
            sound->setVolume(volume);
            sound->play(isLooping);
        }
        _soundsToPlay.clear();
        _soundsVolumes.clear();
        _soundsLooping.clear();
    }
    
    {
        int len = static_cast<int>(_soundsToStop.size());
        for (int i = 0; i < len; ++i)
        {
            Sound* sound = _soundsToStop[i];
            sound->stop();
        }
        _soundsToStop.clear();
    }
    
    {
        int len = static_cast<int>(_soundsToPause.size());
        for (int i = 0; i < len; ++i)
        {
            Sound* sound = _soundsToPause[i];
            sound->pause();
        }
        _soundsToPause.clear();
    }
    
    {
        int len = static_cast<int>(_soundsToResume.size());
        for (int i = 0; i < len; ++i)
        {
            Sound* sound = _soundsToResume[i];
            sound->resume();
        }
        _soundsToResume.clear();
    }
    
    {
        int len = static_cast<int>(_musicStates.size());
        for (int i = 0; i < len; ++i)
        {
            int musicState = _musicStates[i];
            switch (musicState)
            {
                case MUSS_STOP:
                    _music->getSoundInstance()->stop();
                    break;
                case MUSS_PAUSE:
                    _music->getSoundInstance()->pause();
                    break;
                case MUSS_RESUME:
                    if (_music->getSoundInstance()->isPaused())
                    {
                        _music->getSoundInstance()->resume();
                    }
                    break;
                case MUSS_SET_VOLUME:
                    _music->getSoundInstance()->setVolume(_musicVolume);
                    break;
                case MUSS_PLAY_SINGLE:
                    _music->getSoundInstance()->play(false);
                    break;
                case MUSS_PLAY_LOOP:
                    _music->getSoundInstance()->play(true);
                    break;
                case MUSS_DEFAULT:
                default:
                    break;
            }
        }
        _musicStates.clear();
    }
    
    if (_state == GAES_PAUSE)
    {
        _audioEngineHelper->pause();
        _state = GAES_DEFAULT;
    }
}

void GowAudioEngine::pause()
{
    _state = GAES_PAUSE;
    pauseMusic();
    pauseAllSounds();
}

void GowAudioEngine::resume()
{
    _state = GAES_RESUME;
    resumeMusic();
    resumeAllSounds();
}

void GowAudioEngine::loadSound(uint16_t soundID, const char *path, int numInstances)
{
    SoundWrapper* sound = _audioEngineHelper->loadSound(soundID, path, numInstances);
    _sounds.insert(std::make_pair(soundID, sound));
}

void GowAudioEngine::playSound(uint16_t soundID, float volume, bool isLooping)
{
    if (_areSoundsDisabled ||
        _soundsToPlay.size() >= MAX_SOUNDS_TO_PLAY_PER_FRAME ||
        soundID == 0)
    {
        return;
    }
    
    SoundWrapper* soundWrapper = findSound(soundID);
    Sound* sound = soundWrapper->getSoundInstance();
    float volumeClamped = clamp(volume, 0, 1);
    
    _soundsToPlay.push_back(sound);
    _soundsVolumes.push_back(volumeClamped);
    _soundsLooping.push_back(isLooping);
}

void GowAudioEngine::stopSound(uint16_t soundID)
{
    if (_areSoundsDisabled)
    {
        return;
    }
    
    SoundWrapper* soundWrapper = findSound(soundID);
    
    for (int j = 0; j < soundWrapper->getNumInstances(); ++j)
    {
        Sound* sound = soundWrapper->getSoundInstance();
        if (sound->isPlaying())
        {
            _soundsToStop.push_back(sound);
            
            return;
        }
    }
}

void GowAudioEngine::pauseSound(uint16_t soundID)
{
    if (_areSoundsDisabled)
    {
        return;
    }
    
    SoundWrapper* soundWrapper = findSound(soundID);
    
    for (int j = 0; j < soundWrapper->getNumInstances(); ++j)
    {
        Sound* sound = soundWrapper->getSoundInstance();
        if (sound->isPlaying())
        {
            _soundsToPause.push_back(sound);
            
            return;
        }
    }
}

void GowAudioEngine::resumeSound(uint16_t soundID)
{
    if (_areSoundsDisabled)
    {
        return;
    }
    
    SoundWrapper* soundWrapper = findSound(soundID);
    
    for (int j = 0; j < soundWrapper->getNumInstances(); ++j)
    {
        Sound* sound = soundWrapper->getSoundInstance();
        if (sound->isPaused())
        {
            _soundsToResume.push_back(sound);
            
            return;
        }
    }
}

void GowAudioEngine::stopAllSounds()
{
    if (_areSoundsDisabled)
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
    if (_areSoundsDisabled)
    {
        return;
    }
    
    for (std::map<uint16_t, SoundWrapper*>::iterator i = _sounds.begin(); i != _sounds.end(); ++i)
    {
        std::vector<Sound *> sounds = (*i).second->getSounds();
        for (std::vector<Sound *>::iterator i = sounds.begin(); i != sounds.end(); ++i)
        {
            _soundsToPause.push_back((*i));
        }
    }
}

void GowAudioEngine::resumeAllSounds()
{
    if (_areSoundsDisabled)
    {
        return;
    }
    
    for (std::map<uint16_t, SoundWrapper*>::iterator i = _sounds.begin(); i != _sounds.end(); ++i)
    {
        std::vector<Sound *> sounds = (*i).second->getSounds();
        for (std::vector<Sound *>::iterator i = sounds.begin(); i != sounds.end(); ++i)
        {
            _soundsToResume.push_back((*i));
        }
    }
}

void GowAudioEngine::loadMusic(const char *path)
{
    if (_isMusicDisabled)
    {
        return;
    }
    
    if (_music != NULL)
    {
        _music->getSoundInstance()->stop();
        delete _music;
        _music = NULL;
    }
    
    _music = _audioEngineHelper->loadMusic(path);
}

void GowAudioEngine::playMusic(bool isLooping, float volume)
{
    if (_isMusicDisabled || _music == NULL)
    {
        return;
    }
    
    _musicStates.push_back(isLooping ? MUSS_PLAY_LOOP : MUSS_PLAY_SINGLE);
    
    setMusicVolume(volume);
}

void GowAudioEngine::setMusicVolume(float volume)
{
    if (_isMusicDisabled || _music == NULL)
    {
        return;
    }
    
    _musicVolume = clamp(volume, 0, 1);
    _musicStates.push_back(MUSS_SET_VOLUME);
}

void GowAudioEngine::stopMusic()
{
    if (_isMusicDisabled || _music == NULL)
    {
        return;
    }
    
    _musicStates.push_back(MUSS_STOP);
}

void GowAudioEngine::pauseMusic()
{
    if (_isMusicDisabled || _music == NULL)
    {
        return;
    }
    
    _musicStates.push_back(MUSS_PAUSE);
}

void GowAudioEngine::resumeMusic()
{
    if (_isMusicDisabled || _music == NULL)
    {
        return;
    }
    
    _musicStates.push_back(MUSS_RESUME);
}

bool GowAudioEngine::isMusicPlaying()
{
    if (_isMusicDisabled || _music == NULL)
    {
        return false;
    }
    
    return _music->getSoundInstance()->isPlaying();
}

bool GowAudioEngine::isMusicLoaded()
{
    return _music != NULL;
}

bool GowAudioEngine::isMusicDisabled()
{
    return _isMusicDisabled;
}

void GowAudioEngine::setMusicDisabled(bool value)
{
    if (!_isMusicDisabled && value)
    {
        stopMusic();
        _isMusicDisabled = true;
    }
    else if (_isMusicDisabled && !value)
    {
        _isMusicDisabled = false;
        playMusic();
    }
}

bool GowAudioEngine::areSoundsDisabled()
{
    return _areSoundsDisabled;
}

void GowAudioEngine::setSoundsDisabled(bool value)
{
    if (!_areSoundsDisabled && value)
    {
        stopAllSounds();
    }
    
    _areSoundsDisabled = value;
}

SoundWrapper* GowAudioEngine::findSound(uint16_t soundID)
{
    auto q = _sounds.find(soundID);
    
    assert(q != _sounds.end());
    
    SoundWrapper* sound = q->second;
    
    return sound;
}

GowAudioEngine::GowAudioEngine() :
_audioEngineHelper(NULL),
_state(GAES_DEFAULT),
_music(NULL),
_musicVolume(0),
_isMusicDisabled(false),
_areSoundsDisabled(false)
{
    // Empty
}
