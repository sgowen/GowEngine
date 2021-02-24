//
//  GowAudioEngine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <vector>
#include <string>

class Sound;

#define GOW_AUDIO GowAudioEngine::getInstance()

class GowAudioEngine
{
public:
    static GowAudioEngine& getInstance()
    {
        static GowAudioEngine ret = GowAudioEngine();
        return ret;
    }
    
    void render();
    void pause();
    void resume();
    void playSound(uint16_t soundID, float volume = 1.0f, bool isLooping = false);
    void stopSound(uint16_t soundID);
    void pauseSound(uint16_t soundID);
    void resumeSound(uint16_t soundID);
    void stopAllSounds();
    void pauseAllSounds();
    void resumeAllSounds();
    void playMusic(float volume = 1.0f, bool isLooping = true);
    void setMusicVolume(float volume);
    void stopMusic();
    void pauseMusic();
    void resumeMusic();
    bool isMusicPlaying();
    bool isMusicLoaded();
    void reset();
    
private:
    std::vector<Sound*> _soundsToPlay;
    std::vector<Sound*> _soundsToStop;
    std::vector<Sound*> _soundsToPause;
    std::vector<Sound*> _soundsToResume;
    
    GowAudioEngine() {}
    ~GowAudioEngine() {}
    GowAudioEngine(const GowAudioEngine&);
    GowAudioEngine& operator=(const GowAudioEngine&);
};
