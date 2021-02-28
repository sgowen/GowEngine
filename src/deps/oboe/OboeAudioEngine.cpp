//
//  OboeAudioEngine.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "OboeAudioEngine.hpp"

#include "OboeSoundWrapper.hpp"
#include "SimpleMultiPlayer.hpp"

SoundWrapper* OboeAudioEngine::loadSound(std::string filePath, uint8_t numInstances)
{
    return new OboeSoundWrapper(_simpleMultiPlayer, filePath, numInstances);
}

SoundWrapper* OboeAudioEngine::loadMusic(std::string filePath)
{
    return loadSound(filePath);
}

OboeAudioEngine::OboeAudioEngine() : AudioEngine(),
_simpleMultiPlayer(new SimpleMultiPlayer())
{
    _simpleMultiPlayer->setupAudioStream(2);
    _simpleMultiPlayer->startStream();
}

OboeAudioEngine::~OboeAudioEngine()
{
    _simpleMultiPlayer->teardownAudioStream();
    _simpleMultiPlayer->unloadSampleData();
    
    delete _simpleMultiPlayer;
    _simpleMultiPlayer = NULL;
}
