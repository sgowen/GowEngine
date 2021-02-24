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

AndroidSound::AndroidSound(SimpleMultiPlayer* simpleMultiPlayer, const char *filePath, float volume) : Sound(),
_simpleMultiPlayer(simpleMultiPlayer),
_sampleSourceKey(0)
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
    _sampleSourceKey = _simpleMultiPlayer->addSampleSource(source);

    AssetHandlerFactory::destroy(ah);
}

AndroidSound::~AndroidSound()
{
    stop();
    
    _simpleMultiPlayer->unloadSampleSource(_sampleSourceKey);
}

void AndroidSound::play()
{
    _simpleMultiPlayer->play(_sampleSourceKey, _isLooping);
}

void AndroidSound::resume()
{
    if (isPaused())
    {
        _simpleMultiPlayer->resume(_sampleSourceKey);
    }
}

void AndroidSound::pause()
{
    if (isPlaying())
    {
        _simpleMultiPlayer->pause(_sampleSourceKey);
    }
}

void AndroidSound::stop()
{
    _simpleMultiPlayer->stop(_sampleSourceKey);
}

void AndroidSound::setVolume(float volume)
{
    _simpleMultiPlayer->setGain(_sampleSourceKey, volume);
}

bool AndroidSound::isLooping()
{
    return _simpleMultiPlayer->isLooping(_sampleSourceKey);
}

bool AndroidSound::isPlaying()
{
    return _simpleMultiPlayer->isPlaying(_sampleSourceKey);
}

bool AndroidSound::isPaused()
{
    return _simpleMultiPlayer->isPaused(_sampleSourceKey);
}
