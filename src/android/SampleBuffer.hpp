//
//  SampleBuffer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/22/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

struct AudioProperties
{
    int32_t _channelCount;
    int32_t _sampleRate;
};

class WavStreamReader;

class SampleBuffer
{
public:
    SampleBuffer();
    ~SampleBuffer();

    void loadSampleData(WavStreamReader* reader);
    void unloadSampleData();
    void resampleData(int sampleRate);
    AudioProperties getProperties() const;
    float* getSampleData();
    int32_t getNumSampleFrames();

protected:
    AudioProperties _audioProperties;
    float*  _sampleData;
    int32_t _numSamples;
};
