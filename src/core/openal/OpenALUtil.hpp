//
//  OpenALUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/11/23.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

class OpenALSound;

class OpenALUtil
{
public:
    static OpenALUtil& getInstance()
    {
        static OpenALUtil ret = OpenALUtil();
        return ret;
    }
    
    bool openDeviceAndCreateContext();
    bool destroyContextAndCloseDevice();
    
    void create(OpenALSound& sound);
    void destroy(OpenALSound& sound);
    void play(OpenALSound& sound);
    void resume(OpenALSound& sound);
    void pause(OpenALSound& sound);
    void stop(OpenALSound& sound);
    void setVolume(OpenALSound& sound, float volume);
    bool isLooping(OpenALSound& sound);
    bool isPlaying(OpenALSound& sound);
    bool isPaused(OpenALSound& sound);
    void setLooping(OpenALSound& sound, bool isLooping);

    OpenALUtil() {}
    ~OpenALUtil() {}
    OpenALUtil(const OpenALUtil&);
    OpenALUtil& operator=(const OpenALUtil&);
};
