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

#include <assert.h>

AndroidSound::AndroidSound(SimpleMultiPlayer* simpleMultiPlayer, uint16_t soundID, const char *filePath, float volume) : Sound(soundID),
_simpleMultiPlayer(simpleMultiPlayer),
_volume(volume),
_oboeSoundIndex(0),
_isLooping(false),
_isPaused(false)
{
    AssetHandler* ah = AssetHandlerFactory::create();
    
    const FileData fileData = ah->loadAsset(filePath);

    InputStream stream((uint8_t*)fileData._data, fileData._length);
    WavStreamReader reader(&stream);
    reader.parse();

    bool isFormatValid = reader.getNumChannels() == 1;
    assert(isFormatValid);

    SampleBuffer* sampleBuffer = new SampleBuffer();
    sampleBuffer->loadSampleData(&reader);

    SampleSource* source = new SampleSource(sampleBuffer, 0);
    _oboeSoundIndex = _simpleMultiPlayer->addSampleSource(source, sampleBuffer);
    
    AssetHandlerFactory::destroy(ah);
    
    assert(isFormatValid);
}

void AndroidSound::play(bool isLooping)
{
    _isLooping = isLooping;
    _isPaused = false;

    _simpleMultiPlayer->play(_oboeSoundIndex, isLooping);
}

void AndroidSound::resume()
{
    if (_isPaused)
    {
        _simpleMultiPlayer->resume(_oboeSoundIndex);

        _isPaused = false;
    }
}

void AndroidSound::pause()
{
    if (isPlaying())
    {
        _simpleMultiPlayer->pause(_oboeSoundIndex);

        _isPaused = true;
    }
}

void AndroidSound::stop()
{
    _isLooping = false;
    _isPaused = false;

    _simpleMultiPlayer->stop(_oboeSoundIndex);
}

void AndroidSound::setVolume(float volume)
{
    _volume = volume;

    _simpleMultiPlayer->setGain(_oboeSoundIndex, volume);
}

bool AndroidSound::isLooping()
{
    return _isLooping;
}

bool AndroidSound::isPlaying()
{
    return !_isPaused;
}

bool AndroidSound::isPaused()
{
    return _isPaused;
}
