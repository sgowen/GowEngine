//
//  OpenALAudioEngine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/21/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildConstants.hpp>
#if IS_LINUX || IS_APPLE || IS_ANDROID

#include "core/audio/GowAudioEngine.hpp"

class OpenALAudioEngine : public GowAudioEngine
{
    friend class AudioEngineFactory;
    
public:
    virtual SoundWrapper* loadSound(std::string filePath, uint8_t numInstances = 1);
    virtual SoundWrapper* loadMusic(std::string filePath);
    virtual void onPause() {}
    virtual void onResume() {}

private:
    static GowAudioEngine& getInstance()
    {
        static OpenALAudioEngine ret = OpenALAudioEngine();
        return ret;
    }
    
    OpenALAudioEngine();
    ~OpenALAudioEngine();
    OpenALAudioEngine(const OpenALAudioEngine&);
    OpenALAudioEngine& operator=(const OpenALAudioEngine&);
};

#endif /* IS_LINUX */
