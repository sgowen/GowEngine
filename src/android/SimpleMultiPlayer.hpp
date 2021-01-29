//
//  SimpleMultiPlayer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/22/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <oboe/Oboe.h>

#include <vector>

class SampleSource;
class SampleBuffer;

class SimpleMultiPlayer : public oboe::AudioStreamCallback
{
public:
    SimpleMultiPlayer();
    ~SimpleMultiPlayer();

    virtual oboe::DataCallbackResult onAudioReady(oboe::AudioStream *oboeStream, void *audioData, int32_t numFrames) override;
    virtual void onErrorAfterClose(oboe::AudioStream *oboeStream, oboe::Result error) override;
    virtual void onErrorBeforeClose(oboe::AudioStream * oboeStream, oboe::Result error) override;

    void setupAudioStream(int32_t channelCount);
    void teardownAudioStream();
    bool openStream();
    bool startStream();
    int getSampleRate();
    int32_t addSampleSource(SampleSource* source, SampleBuffer* buffer);
    void unloadSampleData();
    void play(int32_t index, bool isLooping);
    void pause(int32_t index);
    void resume(int32_t index);
    void stop(int32_t index);
    void resetAll();
    bool getOutputReset();
    void clearOutputReset();
    void setPan(int index, float pan);
    float getPan(int index);
    void setGain(int index, float gain);
    float getGain(int index);

private:
    std::shared_ptr<oboe::AudioStream> _audioStream;
    int32_t _channelCount;
    int32_t _sampleRate;
    int32_t _numSampleBuffers;
    std::vector<SampleBuffer*> _sampleBuffers;
    std::vector<SampleSource*> _sampleSources;
    bool _outputReset;
};
