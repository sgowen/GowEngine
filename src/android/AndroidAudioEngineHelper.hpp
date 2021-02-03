//
//  AndroidAudioEngineHelper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "AudioEngineHelper.hpp"

class SimpleMultiPlayer;

class AndroidAudioEngineHelper : public AudioEngineHelper
{
    friend class AudioEngineHelperFactory;
    
public:
    virtual void pause() {}
    virtual void resume() {}
    virtual SoundWrapper* loadSound(uint16_t soundID, const char *filePath, int numInstances = 1);
    virtual SoundWrapper* loadMusic(const char* filePath);
    
private:
    SimpleMultiPlayer* _simpleMultiPlayer;
    
    AndroidAudioEngineHelper();
    ~AndroidAudioEngineHelper();
    AndroidAudioEngineHelper(const AndroidAudioEngineHelper&);
    AndroidAudioEngineHelper& operator=(const AndroidAudioEngineHelper&);
};
