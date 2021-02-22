//
//  SimpleMultiPlayer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/22/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

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
    void addSampleSource(uint16_t soundID, SampleSource* source);
    void unloadSampleSource(uint16_t soundID);
    void unloadSampleData();
    void play(uint16_t soundID, bool isLooping);
    bool isPlaying(uint16_t soundID);
    bool isPaused(uint16_t soundID);
    bool isLooping(uint16_t soundID);
    void pause(uint16_t soundID);
    void resume(uint16_t soundID);
    void stop(uint16_t soundID);
    void resetAll();
    bool getOutputReset();
    void clearOutputReset();
    void setPan(uint16_t soundID, float pan);
    float getPan(uint16_t soundID);
    void setGain(uint16_t soundID, float gain);
    float getGain(uint16_t soundID);

private:
    std::shared_ptr<oboe::AudioStream> _audioStream;
    int32_t _channelCount;
    int32_t _sampleRate;
    std::map<int, SampleSource*> _sampleSources;
    bool _outputReset;
};
