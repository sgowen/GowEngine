//
//  OpenALAudioEngine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/21/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/openal/GowAudioEngine.hpp"

class OpenALAudioEngine : public GowAudioEngine
{
public:
    static GowAudioEngine& getInstance()
    {
        static OpenALAudioEngine ret = OpenALAudioEngine();
        return ret;
    }
    
    virtual SoundWrapper* loadSound(std::string filePath, uint8_t numInstances = 1);
    virtual SoundWrapper* loadMusic(std::string filePath);
    virtual void onPause() {}
    virtual void onResume() {}

private:
    OpenALAudioEngine();
    ~OpenALAudioEngine();
    OpenALAudioEngine(const OpenALAudioEngine&);
    OpenALAudioEngine& operator=(const OpenALAudioEngine&);
};
