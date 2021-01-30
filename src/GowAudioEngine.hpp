//
//  GowAudioEngine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <map>
#include <vector>
#include <string>

enum GowAudioEngineState
{
    GAES_DEFAULT,
    GAES_PAUSE,
    GAES_RESUME
};

enum MusicState
{
    MUSS_DEFAULT,
    MUSS_STOP,
    MUSS_PAUSE,
    MUSS_RESUME,
    MUSS_SET_VOLUME,
    MUSS_PLAY_SINGLE,
    MUSS_PLAY_LOOP
};

class SoundWrapper;
class Sound;
class AudioEngineHelper;

#define GOW_AUDIO GowAudioEngine::getInstance()

class GowAudioEngine
{
public:
    static GowAudioEngine& getInstance();
    
    void createDeviceDependentResources();
    void releaseDeviceDependentResources();
    void render();
    void pause();
    void resume();
    void loadSound(uint16_t soundID, const char *path, int numInstances = 1);
    void playSound(uint16_t soundID, float volume = 1.0f, bool isLooping = false);
    void stopSound(uint16_t soundID);
    void pauseSound(uint16_t soundID);
    void resumeSound(uint16_t soundID);
    void stopAllSounds();
    void pauseAllSounds();
    void resumeAllSounds();
    void loadMusic(const char *path);
    void playMusic(bool isLooping = true, float volume = 1.0f);
    void setMusicVolume(float volume);
    void stopMusic();
    void pauseMusic();
    void resumeMusic();
    bool isMusicPlaying();
    bool isMusicLoaded();
    bool isMusicDisabled();
    void setMusicDisabled(bool isMusicDisabled);
    bool areSoundsDisabled();
    void setSoundsDisabled(bool areSoundsDisabled);
    void reset();
    
private:
    AudioEngineHelper* _audioEngineHelper;
    GowAudioEngineState _state;
    std::map<uint16_t, SoundWrapper*> _sounds;
    std::vector<Sound*> _soundsToPlay;
    std::vector<Sound*> _soundsToStop;
    std::vector<Sound*> _soundsToPause;
    std::vector<Sound*> _soundsToResume;
    std::vector<float> _soundsVolumes;
    std::vector<bool> _soundsLooping;
    SoundWrapper* _music;
    std::vector<int> _musicStates;
    float _musicVolume;
    bool _isMusicDisabled;
    bool _areSoundsDisabled;
    
    SoundWrapper* findSound(uint16_t soundID);
    
    GowAudioEngine();
    ~GowAudioEngine() {}
    GowAudioEngine(const GowAudioEngine&);
    GowAudioEngine& operator=(const GowAudioEngine&);
};
