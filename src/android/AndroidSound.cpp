//
//  AndroidSound.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/21/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "AndroidSound.hpp"

#include "SimpleMultiPlayer.hpp"
#include "AssetHandler.hpp"
#include "AssetHandlerFactory.hpp"
#include "FileData.hpp"
#include "InputStream.hpp"
#include "WavStreamReader.hpp"
#include "SampleBuffer.hpp"
#include "SampleSource.hpp"
#include "StringUtil.hpp"

#include <assert.h>

AndroidSound::AndroidSound(SimpleMultiPlayer* simpleMultiPlayer, uint16_t soundID, const char *filePath, float volume) : Sound(soundID),
_simpleMultiPlayer(simpleMultiPlayer)
{
    AssetHandler* ah = AssetHandlerFactory::create();
    
    const FileData fileData = ah->loadAsset(filePath);

    InputStream stream((uint8_t*)fileData._data, fileData._length);
    WavStreamReader reader(&stream);
    reader.parse();

    bool isFormatValid = reader.getNumChannels() == 1;
    if (!isFormatValid)
    {
        LOG("%s is in an invalid format!", filePath);
    }

    assert(isFormatValid);

    SampleBuffer* sampleBuffer = new SampleBuffer();
    sampleBuffer->loadSampleData(&reader);

    SampleSource* source = new SampleSource(sampleBuffer, 0);
    _simpleMultiPlayer->addSampleSource(soundID, source);

    AssetHandlerFactory::destroy(ah);
}

AndroidSound::~AndroidSound()
{
    stop();
    
    _simpleMultiPlayer->unloadSampleSource(_soundID);
}

void AndroidSound::play(bool isLooping)
{
    _simpleMultiPlayer->play(_soundID, isLooping);
}

void AndroidSound::resume()
{
    if (isPaused())
    {
        _simpleMultiPlayer->resume(_soundID);
    }
}

void AndroidSound::pause()
{
    if (isPlaying())
    {
        _simpleMultiPlayer->pause(_soundID);
    }
}

void AndroidSound::stop()
{
    _simpleMultiPlayer->stop(_soundID);
}

void AndroidSound::setVolume(float volume)
{
    _simpleMultiPlayer->setGain(_soundID, volume);
}

bool AndroidSound::isLooping()
{
    return _simpleMultiPlayer->isLooping(_soundID);
}

bool AndroidSound::isPlaying()
{
    return _simpleMultiPlayer->isPlaying(_soundID);
}

bool AndroidSound::isPaused()
{
    return _simpleMultiPlayer->isPaused(_soundID);
}
