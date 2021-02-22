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
#include "GowUtil.hpp"

using namespace oboe;

constexpr int32_t kBufferSizeInBursts = 2; // Use 2 bursts as the buffer size (double buffer)

SimpleMultiPlayer::SimpleMultiPlayer() :
_channelCount(0),
_outputReset(false)
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

    for (auto& pair: _sampleSources)
    {
        pair.second->mixAudio((float*)audioData, _channelCount, numFrames, pair.second->isLooping());
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
    _channelCount = channelCount;

    openStream();
}

void SimpleMultiPlayer::teardownAudioStream()
{
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

void SimpleMultiPlayer::addSampleSource(uint16_t soundID, SampleSource* source)
{
    source->buffer()->resampleData(_sampleRate);

    _sampleSources.emplace(soundID, source);
}

void SimpleMultiPlayer::unloadSampleSource(uint16_t soundID)
{
    auto q = _sampleSources.find(soundID);
    assert(q != _sampleSources.end());
    delete q->second;
    _sampleSources.erase(q);
}

void SimpleMultiPlayer::unloadSampleData()
{
    resetAll();
    GowUtil::cleanUpMapOfPointers(_sampleSources);
}

void SimpleMultiPlayer::play(uint16_t soundID, bool isLooping)
{
    auto q = _sampleSources.find(soundID);
    assert(q != _sampleSources.end());
    
    q->second->setPlayMode(isLooping);
}

bool SimpleMultiPlayer::isPlaying(uint16_t soundID)
{
    auto q = _sampleSources.find(soundID);
    assert(q != _sampleSources.end());
    
    return q->second->isPlaying();
}

bool SimpleMultiPlayer::isPaused(uint16_t soundID)
{
    auto q = _sampleSources.find(soundID);
    assert(q != _sampleSources.end());
    
    return q->second->isPaused();
}

bool SimpleMultiPlayer::isLooping(uint16_t soundID)
{
    auto q = _sampleSources.find(soundID);
    assert(q != _sampleSources.end());
    
    return q->second->isLooping();
}

void SimpleMultiPlayer::pause(uint16_t soundID)
{
    auto q = _sampleSources.find(soundID);
    assert(q != _sampleSources.end());
    
    q->second->setPauseMode();
}

void SimpleMultiPlayer::resume(uint16_t soundID)
{
    auto q = _sampleSources.find(soundID);
    assert(q != _sampleSources.end());
    
    q->second->setResumeMode();
}

void SimpleMultiPlayer::stop(uint16_t soundID)
{
    auto q = _sampleSources.find(soundID);
    assert(q != _sampleSources.end());
    
    q->second->setStopMode();
}

void SimpleMultiPlayer::resetAll()
{
    for (auto& pair: _sampleSources)
    {
        pair.second->setStopMode();
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

void SimpleMultiPlayer::setPan(uint16_t soundID, float pan)
{
    auto q = _sampleSources.find(soundID);
    assert(q != _sampleSources.end());
    
    q->second->setPan(pan);
}

float SimpleMultiPlayer::getPan(uint16_t soundID)
{
    auto q = _sampleSources.find(soundID);
    assert(q != _sampleSources.end());
    
    return q->second->getPan();
}

void SimpleMultiPlayer::setGain(uint16_t soundID, float gain)
{
    auto q = _sampleSources.find(soundID);
    assert(q != _sampleSources.end());
    
    q->second->setGain(gain);
}

float SimpleMultiPlayer::getGain(uint16_t soundID)
{
    auto q = _sampleSources.find(soundID);
    assert(q != _sampleSources.end());
    
    return q->second->getGain();
}
