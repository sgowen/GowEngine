//
//  AndroidAudioEngine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "AudioEngine.hpp"

class SimpleMultiPlayer;

class AndroidAudioEngine : public AudioEngine
{
    friend class AudioEngineFactory;
    
public:
    virtual SoundWrapper* loadSound(const char *filePath, uint8_t numInstances = 1);
    virtual SoundWrapper* loadMusic(const char* filePath);
    virtual void onPause() {}
    virtual void onResume() {}
    
private:
    static AudioEngine* getInstance()
    {
        static AndroidAudioEngine ret = AndroidAudioEngine();
        return &ret;
    }
    
    SimpleMultiPlayer* _simpleMultiPlayer;
    
    AndroidAudioEngine();
    ~AndroidAudioEngine();
    AndroidAudioEngine(const AndroidAudioEngine&);
    AndroidAudioEngine& operator=(const AndroidAudioEngine&);
};
