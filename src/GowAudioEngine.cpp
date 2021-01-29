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

GowAudioEngine* GowAudioEngine::s_instance = NULL;

void GowAudioEngine::create()
{
    assert(!s_instance);
    
    s_instance = new GowAudioEngine();
}

GowAudioEngine* GowAudioEngine::getInstance()
{
    return s_instance;
}

void GowAudioEngine::destroy()
{
    assert(s_instance);
    
    delete s_instance;
    s_instance = NULL;
}

void GowAudioEngine::render()
{
    if (_state == AudioEngineState_Resume)
    {
        _audioEngineHelper->resume();
        _state = AudioEngineState_None;
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
                case MusicState_Stop:
                    _music->getSoundInstance()->stop();
                    break;
                case MusicState_Pause:
                    _music->getSoundInstance()->pause();
                    break;
                case MusicState_Resume:
                    if (_music->getSoundInstance()->isPaused())
                    {
                        _music->getSoundInstance()->resume();
                    }
                    break;
                case MusicState_SetVolume:
                    _music->getSoundInstance()->setVolume(_musicVolume);
                    break;
                case MusicState_Play:
                    _music->getSoundInstance()->play(_isMusicLooping);
                    _music->getSoundInstance()->setVolume(_musicVolume);
                    break;
                case MusicState_None:
                default:
                    break;
            }
        }
        _musicStates.clear();
    }
    
    if (_state == AudioEngineState_Pause)
    {
        _audioEngineHelper->pause();
        _state = AudioEngineState_None;
    }
}

void GowAudioEngine::pause()
{
    _state = AudioEngineState_Pause;
    pauseMusic();
    pauseAllSounds();
}

void GowAudioEngine::resume()
{
    _state = AudioEngineState_Resume;
    resumeMusic();
    resumeAllSounds();
}

void GowAudioEngine::loadFromAssets()
{
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

void GowAudioEngine::loadSound(uint16_t soundID, const char *path, int numInstances)
{
    SoundWrapper* sound = _audioEngineHelper->loadSound(soundID, path, numInstances);
    _sounds.insert(std::make_pair(soundID, sound));
}

void GowAudioEngine::playSound(uint16_t soundID, float volume, bool isLooping)
{
    if (_areSoundsDisabled || _soundsToPlay.size() >= MAX_SOUNDS_TO_PLAY_PER_FRAME)
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
    
    if (_music)
    {
        _music->getSoundInstance()->stop();
        
        delete _music;
    }
    _music = _audioEngineHelper->loadMusic(path);
}

void GowAudioEngine::playMusic(bool isLooping, float volume)
{
    if (_isMusicDisabled)
    {
        return;
    }
    
    if (_music)
    {
        _musicVolume = clamp(volume, 0, 1);
        _isMusicLooping = isLooping;
        _musicStates.push_back(MusicState_Play);
    }
}

void GowAudioEngine::setMusicVolume(float volume)
{
    if (_isMusicDisabled)
    {
        return;
    }
    
    if (_music)
    {
        _musicVolume = clamp(volume, 0, 1);
        _musicStates.push_back(MusicState_SetVolume);
    }
}

void GowAudioEngine::stopMusic()
{
    if (_isMusicDisabled)
    {
        return;
    }
    
    if (_music)
    {
        _musicStates.push_back(MusicState_Stop);
    }
}

void GowAudioEngine::pauseMusic()
{
    if (_isMusicDisabled)
    {
        return;
    }
    
    if (_music)
    {
        _musicStates.push_back(MusicState_Pause);
    }
}

void GowAudioEngine::resumeMusic()
{
    if (_isMusicDisabled)
    {
        return;
    }
    
    if (_music)
    {
        _musicStates.push_back(MusicState_Resume);
    }
}

bool GowAudioEngine::isMusicPlaying()
{
    if (_music)
    {
        return _music->getSoundInstance()->isPlaying();
    }
    
    return false;
}

bool GowAudioEngine::isMusicLoaded()
{
    return _music ? true : false;
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
_audioEngineHelper(AudioEngineHelperFactory::create()),
_state(AudioEngineState_None),
_music(NULL),
_musicVolume(0),
_isMusicLooping(false),
_isMusicDisabled(false),
_areSoundsDisabled(false)
{
    // Empty
}

GowAudioEngine::~GowAudioEngine()
{
    if (_music != NULL)
    {
        delete _music;
        _music = NULL;
    }
    
    GowUtil::cleanUpMapOfPointers(_sounds);
    
    AudioEngineHelperFactory::destroy(_audioEngineHelper);
}
