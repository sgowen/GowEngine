//
//  ObjectALAudioEngine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "AudioEngine.hpp"

class ObjectALAudioEngine : public AudioEngine
{
    friend class AudioEngineFactory;
    
public:
    virtual SoundWrapper* loadSound(std::string filePath, uint8_t numInstances = 1);
    virtual SoundWrapper* loadMusic(std::string filePath);
    virtual void onPause();
    virtual void onResume();

private:
    static AudioEngine& getInstance()
    {
        static ObjectALAudioEngine ret = ObjectALAudioEngine();
        return ret;
    }
    
    ObjectALAudioEngine();
    virtual ~ObjectALAudioEngine();
    ObjectALAudioEngine(const ObjectALAudioEngine&);
    ObjectALAudioEngine& operator=(const ObjectALAudioEngine&);
};
