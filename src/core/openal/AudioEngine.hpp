//
//  AudioEngine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/21/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>
#include <vector>
#include <string>

class AudioEngine
{
public:
    static void create();
    static AudioEngine& getInstance();
    static void destroy();
    
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

private:
    static AudioEngine* s_instance;
    
    std::vector<uint32_t> _soundsToPlay;
    std::vector<uint32_t> _soundsToStop;
    std::vector<uint32_t> _soundsToPause;
    std::vector<uint32_t> _soundsToResume;
    
    AudioEngine();
    ~AudioEngine();
    AudioEngine(const AudioEngine&);
    AudioEngine& operator=(const AudioEngine&);
};
