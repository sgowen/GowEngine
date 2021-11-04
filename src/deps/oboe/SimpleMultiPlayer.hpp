//
//  SimpleMultiPlayer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/22/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildConstants.hpp>
#if IS_ANDROID

#include <oboe/Oboe.h>

#include <map>

class SampleSource;
class SampleBuffer;

class SimpleMultiPlayer : public oboe::AudioStreamCallback
{
public:
    SimpleMultiPlayer();

    virtual oboe::DataCallbackResult onAudioReady(oboe::AudioStream *oboeStream, void *audioData, int32_t numFrames) override;
    virtual void onErrorAfterClose(oboe::AudioStream *oboeStream, oboe::Result error) override;
    virtual void onErrorBeforeClose(oboe::AudioStream * oboeStream, oboe::Result error) override;

    void setupAudioStream(int32_t channelCount);
    void teardownAudioStream();
    bool openStream();
    bool startStream();
    int getSampleRate();
    uint32_t addSampleSource(SampleSource* source);
    void unloadSampleSource(uint32_t key);
    void unloadSampleData();
    void play(uint32_t key, bool isLooping);
    bool isPlaying(uint32_t key);
    bool isPaused(uint32_t key);
    bool isLooping(uint32_t key);
    void pause(uint32_t key);
    void resume(uint32_t key);
    void stop(uint32_t key);
    void resetAll();
    bool getOutputReset();
    void clearOutputReset();
    void setPan(uint32_t key, float pan);
    float getPan(uint32_t key);
    void setGain(uint32_t key, float gain);
    float getGain(uint32_t key);

private:
    std::shared_ptr<oboe::AudioStream> _audioStream;
    int32_t _channelCount;
    int32_t _sampleRate;
    std::map<int, SampleSource*> _sampleSources;
    bool _outputReset;
    uint32_t _sampleSourceKey;
    
    SampleSource* sampleSource(uint32_t key);
};

#endif /* IS_ANDROID */
