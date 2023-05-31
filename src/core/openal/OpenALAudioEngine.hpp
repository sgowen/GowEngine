//
//  OpenALAudioEngine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/21/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>
#include <vector>
#include <string>

class OpenALSoundWrapper;
class OpenALSound;

class OpenALAudioEngine
{
public:
    static OpenALAudioEngine& getInstance()
    {
        static OpenALAudioEngine ret = OpenALAudioEngine();
        return ret;
    }
    
    OpenALSoundWrapper* loadSound(std::string filePath, uint8_t numInstances = 1);
    OpenALSoundWrapper* loadMusic(std::string filePath);
    
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

private:
    std::vector<OpenALSound*> _soundsToPlay;
    std::vector<OpenALSound*> _soundsToStop;
    std::vector<OpenALSound*> _soundsToPause;
    std::vector<OpenALSound*> _soundsToResume;
    
    OpenALAudioEngine();
    ~OpenALAudioEngine();
    OpenALAudioEngine(const OpenALAudioEngine&);
    OpenALAudioEngine& operator=(const OpenALAudioEngine&);
};
