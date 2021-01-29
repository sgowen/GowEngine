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

void AndroidAudioEngineHelper::pause()
{
    _simpleMultiPlayer->teardownAudioStream();
}

void AndroidAudioEngineHelper::resume()
{
    _simpleMultiPlayer->startStream();
}

SoundWrapper* AndroidAudioEngineHelper::loadSound(uint16_t soundID, const char *filePath, int numInstances)
{
    return new AndroidSoundWrapper(_simpleMultiPlayer, soundID, filePath, numInstances);
}

SoundWrapper* AndroidAudioEngineHelper::loadMusic(const char* filePath)
{
    return loadSound(1337, filePath);
}

AndroidAudioEngineHelper::AndroidAudioEngineHelper() : AudioEngineHelper(),
_simpleMultiPlayer(new SimpleMultiPlayer())
{
    _simpleMultiPlayer->setupAudioStream(2);
    _simpleMultiPlayer->startStream();
}

AndroidAudioEngineHelper::~AndroidAudioEngineHelper()
{
    _simpleMultiPlayer->unloadSampleData();
    
    delete _simpleMultiPlayer;
    _simpleMultiPlayer = NULL;
}
