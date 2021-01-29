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

enum AudioEngineState
{
    AudioEngineState_None = 0,
    AudioEngineState_Pause,
    AudioEngineState_Resume
};

enum MusicState
{
    MusicState_None = 0,
    MusicState_Stop,
    MusicState_Pause,
    MusicState_Resume,
    MusicState_SetVolume,
    MusicState_Play
};

class SoundWrapper;
class Sound;
class AudioEngineHelper;

#define GOW_AUDIO GowAudioEngine::getInstance()

class GowAudioEngine
{
public:
    static void create();
    static GowAudioEngine* getInstance();
    static void destroy();
    
    void render();
    void pause();
    void resume();
    void loadFromAssets();
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
    static GowAudioEngine* s_instance;
    
    AudioEngineHelper* _audioEngineHelper;
    int _state;
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
    bool _isMusicLooping;
    bool _isMusicDisabled;
    bool _areSoundsDisabled;
    
    SoundWrapper* findSound(uint16_t soundID);
    
    GowAudioEngine();
    ~GowAudioEngine();
    GowAudioEngine(const GowAudioEngine&);
    GowAudioEngine& operator=(const GowAudioEngine&);
};
