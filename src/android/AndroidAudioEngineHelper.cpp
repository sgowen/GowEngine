//
//  AndroidAudioEngineHelper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "AndroidAudioEngineHelper.hpp"

#include "AndroidSoundWrapper.hpp"
#include "SimpleMultiPlayer.hpp"

SoundWrapper* AndroidAudioEngineHelper::loadSound(const char *filePath, uint8_t numInstances)
{
    return new AndroidSoundWrapper(_simpleMultiPlayer, filePath, numInstances);
}

SoundWrapper* AndroidAudioEngineHelper::loadMusic(const char* filePath)
{
    return loadSound(filePath);
}

AndroidAudioEngineHelper::AndroidAudioEngineHelper() : AudioEngineHelper(),
_simpleMultiPlayer(new SimpleMultiPlayer())
{
    _simpleMultiPlayer->setupAudioStream(2);
    _simpleMultiPlayer->startStream();
}

AndroidAudioEngineHelper::~AndroidAudioEngineHelper()
{
    _simpleMultiPlayer->teardownAudioStream();
    _simpleMultiPlayer->unloadSampleData();
    
    delete _simpleMultiPlayer;
    _simpleMultiPlayer = NULL;
}
