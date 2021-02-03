//
//  SimpleMultiPlayer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/22/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "SimpleMultiPlayer.hpp"

#include "SampleSource.hpp"
#include "SampleBuffer.hpp"
#include "StringUtil.hpp"

using namespace oboe;

constexpr int32_t kBufferSizeInBursts = 2; // Use 2 bursts as the buffer size (double buffer)

SimpleMultiPlayer::SimpleMultiPlayer() :
_channelCount(0),
_outputReset(false),
_numSampleBuffers(0)
{
    // Empty
}

DataCallbackResult SimpleMultiPlayer::onAudioReady(AudioStream *oboeStream, void *audioData, int32_t numFrames)
{
    StreamState streamState = oboeStream->getState();
    if (streamState != StreamState::Open && streamState != StreamState::Started)
    {
        LOG("SimpleMultiPlayer streamState: %d", streamState);
    }
    if (streamState == StreamState::Disconnected)
    {
        LOG("SimpleMultiPlayer streamState::Disconnected");
    }

    memset(audioData, 0, numFrames * _channelCount * sizeof(float));

    for (int32_t index = 0; index < _numSampleBuffers; index++)
    {
        if (_sampleSources[index]->isPlaying())
        {
            _sampleSources[index]->mixAudio((float*)audioData, _channelCount, numFrames, _sampleSources[index]->isLooping());
        }
    }

    return DataCallbackResult::Continue;
}

void SimpleMultiPlayer::onErrorAfterClose(AudioStream *oboeStream, Result error)
{
    LOG("SimpleMultiPlayer ==== onErrorAfterClose() error:%d", error);

    resetAll();
    if (openStream() && startStream())
    {
        _outputReset = true;
    }
}

void SimpleMultiPlayer::onErrorBeforeClose(AudioStream *, Result error)
{
    LOG("SimpleMultiPlayer ==== onErrorBeforeClose() error:%d", error);
}

void SimpleMultiPlayer::setupAudioStream(int32_t channelCount)
{
    LOG("SimpleMultiPlayer setupAudioStream()");
    _channelCount = channelCount;

    openStream();
}

void SimpleMultiPlayer::teardownAudioStream()
{
    LOG("SimpleMultiPlayer teardownAudioStream()");
    // tear down the player
    if (_audioStream != NULL)
    {
        _audioStream->stop();
        _audioStream->close();
        _audioStream.reset();
    }
}

bool SimpleMultiPlayer::openStream()
{
    LOG("SimpleMultiPlayer openStream()");

    // Create an audio stream
    AudioStreamBuilder builder;
    builder.setChannelCount(_channelCount);
    // we will resample source data to device rate, so take default sample rate
    builder.setCallback(this);
    builder.setPerformanceMode(PerformanceMode::LowLatency);
    builder.setSharingMode(SharingMode::Exclusive);
    builder.setSampleRateConversionQuality(SampleRateConversionQuality::Medium);

    Result result = builder.openStream(_audioStream);
    if (result != Result::OK)
    {
        LOG("SimpleMultiPlayer openStream failed. Error: %s", convertToText(result));
        return false;
    }

    // Reduce stream latency by setting the buffer size to a multiple of the burst size
    // Note: this will fail with ErrorUnimplemented if we are using a callback with OpenSL ES
    // See oboe::AudioStreamBuffered::setBufferSizeInFrames
    result = _audioStream->setBufferSizeInFrames(_audioStream->getFramesPerBurst() * kBufferSizeInBursts);
    if (result != Result::OK)
    {
        LOG("SimpleMultiPlayer setBufferSizeInFrames failed. Error: %s", convertToText(result));
    }

    _sampleRate = _audioStream->getSampleRate();

    return true;
}

bool SimpleMultiPlayer::startStream()
{
    Result result = _audioStream->requestStart();
    if (result != Result::OK)
    {
        LOG("SimpleMultiPlayer requestStart failed. Error: %s", convertToText(result));
        return false;
    }

    return true;
}

int SimpleMultiPlayer::getSampleRate()
{
    return _sampleRate;
}

int32_t SimpleMultiPlayer::addSampleSource(SampleSource* source, SampleBuffer* buffer)
{
    buffer->resampleData(_sampleRate);

    _sampleBuffers.push_back(buffer);
    _sampleSources.push_back(source);
    
    return _numSampleBuffers++;
}

void SimpleMultiPlayer::unloadSampleData()
{
    LOG("SimpleMultiPlayer unloadSampleData()");
    resetAll();

    for (int32_t bufferIndex = 0; bufferIndex < _numSampleBuffers; bufferIndex++)
    {
        delete _sampleBuffers[bufferIndex];
        delete _sampleSources[bufferIndex];
    }

    _sampleBuffers.clear();
    _sampleSources.clear();

    _numSampleBuffers = 0;
}

void SimpleMultiPlayer::play(int32_t index, bool isLooping)
{
    assert(index < _numSampleBuffers);
    
    _sampleSources[index]->setPlayMode(isLooping);
}

bool SimpleMultiPlayer::isPlaying(int32_t index)
{
    return _sampleSources[index]->isPlaying();
}

bool SimpleMultiPlayer::isPaused(int32_t index)
{
    return _sampleSources[index]->isPaused();
}

bool SimpleMultiPlayer::isLooping(int32_t index)
{
    return _sampleSources[index]->isLooping();
}

void SimpleMultiPlayer::pause(int32_t index)
{
    assert(index < _numSampleBuffers);
    
    _sampleSources[index]->setPauseMode();
}

void SimpleMultiPlayer::resume(int32_t index)
{
    assert(index < _numSampleBuffers);
    
    _sampleSources[index]->setResumeMode();
}

void SimpleMultiPlayer::stop(int32_t index)
{
    assert(index < _numSampleBuffers);
    
    _sampleSources[index]->setStopMode();
}

void SimpleMultiPlayer::resetAll()
{
    for (int32_t bufferIndex = 0; bufferIndex < _numSampleBuffers; bufferIndex++)
    {
        _sampleSources[bufferIndex]->setStopMode();
    }
}

bool SimpleMultiPlayer::getOutputReset()
{
    return _outputReset;
}

void SimpleMultiPlayer::clearOutputReset()
{
    _outputReset = false;
}

void SimpleMultiPlayer::setPan(int index, float pan)
{
    _sampleSources[index]->setPan(pan);
}

float SimpleMultiPlayer::getPan(int index)
{
    return _sampleSources[index]->getPan();
}

void SimpleMultiPlayer::setGain(int index, float gain)
{
    _sampleSources[index]->setGain(gain);
}

float SimpleMultiPlayer::getGain(int index)
{
    return _sampleSources[index]->getGain();
}
