//
//  OboeAudioEngine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "PlatformMacros.hpp"
#if IS_ANDROID

#include "AudioEngine.hpp"

class SimpleMultiPlayer;

class OboeAudioEngine : public AudioEngine
{
    friend class AudioEngineFactory;
    
public:
    virtual SoundWrapper* loadSound(std::string filePath, uint8_t numInstances = 1);
    virtual SoundWrapper* loadMusic(std::string filePath);
    virtual void onPause() {}
    virtual void onResume() {}
    
private:
    static AudioEngine* getInstance()
    {
        static OboeAudioEngine ret = OboeAudioEngine();
        return &ret;
    }
    
    SimpleMultiPlayer* _simpleMultiPlayer;
    
    OboeAudioEngine();
    ~OboeAudioEngine();
    OboeAudioEngine(const OboeAudioEngine&);
    OboeAudioEngine& operator=(const OboeAudioEngine&);
};

#endif /* IS_ANDROID */
