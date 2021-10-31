//
//  GowAudioEngine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>
#include <vector>
#include <string>

class SoundWrapper;
class Sound;

class GowAudioEngine
{    
public:
    virtual SoundWrapper* loadSound(std::string filePath, uint8_t numInstances = 1) = 0;
    virtual SoundWrapper* loadMusic(std::string filePath) = 0;
    virtual void onPause() = 0;
    virtual void onResume() = 0;
    
    void pause();
    void resume();
    void render();
    void playSound(std::string soundID, float volume = 1.0f, bool isLooping = false);
    void stopSound(std::string soundID);
    void pauseSound(std::string soundID);
    void resumeSound(std::string soundID);
    void stopAllSounds();
    void pauseAllSounds();
    void resumeAllSounds();
    void playMusic(float volume = 1.0f, bool isLooping = true);
    void setMusicVolume(float volume);
    void stopMusic();
    void pauseMusic();
    void resumeMusic();
    bool isMusicPlaying();
    
protected:
    GowAudioEngine();
    virtual ~GowAudioEngine();
    
private:
    std::vector<Sound*> _soundsToPlay;
    std::vector<Sound*> _soundsToStop;
    std::vector<Sound*> _soundsToPause;
    std::vector<Sound*> _soundsToResume;
};
