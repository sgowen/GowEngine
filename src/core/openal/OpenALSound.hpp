//
//  OpenALSound.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

#include <AL/al.h>

class OpenALSound
{
public:
    OpenALSound(std::string filePath, float volume = 1.0f);
    ~OpenALSound();

    void play();
    void resume();
    void pause();
    void stop();
    void setVolume(float volume);
    bool isLooping();
    bool isPlaying();
    bool isPaused();
    void setLooping(bool isLooping);

private:
    ALuint _buf;
    ALuint _src;
    bool _isLooping;
};
