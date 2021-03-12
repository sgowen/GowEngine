//
//  SimpleMultiPlayer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/22/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>
#if IS_ANDROID

using namespace oboe;

constexpr int32_t kBufferSizeInBursts = 2;

SimpleMultiPlayer::SimpleMultiPlayer() :
_channelCount(0),
_outputReset(false),
_sampleSourceKey(1)
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
    if (_audioStream != NULL)
    {
        _audioStream->stop();
        _audioStream->close();
        _audioStream.reset();
    }
}

bool SimpleMultiPlayer::openStream()
{
    AudioStreamBuilder builder;
    builder.setChannelCount(_channelCount);
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

uint32_t SimpleMultiPlayer::addSampleSource(SampleSource* source)
{
    source->buffer()->resampleData(_sampleRate);
    
    _sampleSources.emplace(_sampleSourceKey, source);
    
    return _sampleSourceKey++;
}

void SimpleMultiPlayer::unloadSampleSource(uint32_t key)
{
    auto q = _sampleSources.find(key);
    assert(q != _sampleSources.end());
    delete q->second;
    _sampleSources.erase(q);
}

void SimpleMultiPlayer::unloadSampleData()
{
    resetAll();
    STLUtil::cleanUpMapOfPointers(_sampleSources);
}

void SimpleMultiPlayer::play(uint32_t key, bool isLooping)
{
    sampleSource(key)->setPlayMode(isLooping);
}

bool SimpleMultiPlayer::isPlaying(uint32_t key)
{
    sampleSource(key)->isPlaying();
}

bool SimpleMultiPlayer::isPaused(uint32_t key)
{
    sampleSource(key)->isPaused();
}

bool SimpleMultiPlayer::isLooping(uint32_t key)
{
    sampleSource(key)->isLooping();
}

void SimpleMultiPlayer::pause(uint32_t key)
{
    sampleSource(key)->setPauseMode();
}

void SimpleMultiPlayer::resume(uint32_t key)
{
    sampleSource(key)->setResumeMode();
}

void SimpleMultiPlayer::stop(uint32_t key)
{
    sampleSource(key)->setStopMode();
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

void SimpleMultiPlayer::setPan(uint32_t key, float pan)
{
    sampleSource(key)->setPan(pan);
}

float SimpleMultiPlayer::getPan(uint32_t key)
{
    return sampleSource(key)->getPan();
}

void SimpleMultiPlayer::setGain(uint32_t key, float gain)
{
    sampleSource(key)->setGain(gain);
}

float SimpleMultiPlayer::getGain(uint32_t key)
{
    return sampleSource(key)->getGain();
}

SampleSource* SimpleMultiPlayer::sampleSource(uint32_t key)
{
    auto q = _sampleSources.find(key);
    assert(q != _sampleSources.end());
    
    return q->second;
}

#endif /* IS_ANDROID */
