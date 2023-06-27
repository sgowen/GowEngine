//
//  OpenALUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/11/23.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

struct Sound;

class OpenALUtil
{
public:
    static OpenALUtil& getInstance()
    {
        static OpenALUtil ret = OpenALUtil();
        return ret;
    }
    
    void openDeviceAndCreateContext();
    void destroyContextAndCloseDevice();
    
    void loadSound(Sound& sound);
    void unloadSound(Sound& sound);
    
    void play(uint32_t alHandle);
    void resume(uint32_t alHandle);
    void pause(uint32_t alHandle);
    void stop(uint32_t alHandle);
    void setVolume(uint32_t alHandle, float volume);
    bool isLooping(uint32_t alHandle);
    void setLooping(uint32_t alHandle, bool isLooping);

private:
    bool isPlaying(uint32_t alHandle);
    bool isPaused(uint32_t alHandle);
    
    OpenALUtil() {}
    ~OpenALUtil() {}
    OpenALUtil(const OpenALUtil&);
    OpenALUtil& operator=(const OpenALUtil&);
};
