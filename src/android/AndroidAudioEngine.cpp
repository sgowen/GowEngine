//
//  AndroidAudioEngine.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "AndroidAudioEngine.hpp"

#include "AndroidSoundWrapper.hpp"
#include "SimpleMultiPlayer.hpp"

SoundWrapper* AndroidAudioEngine::loadSound(const char *filePath, uint8_t numInstances)
{
    return new AndroidSoundWrapper(_simpleMultiPlayer, filePath, numInstances);
}

SoundWrapper* AndroidAudioEngine::loadMusic(const char* filePath)
{
    return loadSound(filePath);
}

AndroidAudioEngine::AndroidAudioEngine() : AudioEngine(),
_simpleMultiPlayer(new SimpleMultiPlayer())
{
    _simpleMultiPlayer->setupAudioStream(2);
    _simpleMultiPlayer->startStream();
}

AndroidAudioEngine::~AndroidAudioEngine()
{
    _simpleMultiPlayer->teardownAudioStream();
    _simpleMultiPlayer->unloadSampleData();
    
    delete _simpleMultiPlayer;
    _simpleMultiPlayer = NULL;
}
