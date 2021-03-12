//
//  SampleSource.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/22/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/PlatformMacros.hpp"
#if IS_ANDROID

#include <stdint.h>

class SampleBuffer;

class SampleSource
{
public:
    static constexpr float PAN_HARDLEFT = -1.0f;
    static constexpr float PAN_HARDRIGHT = 1.0f;
    static constexpr float PAN_CENTER = 0.0f;

    SampleSource(SampleBuffer* sampleBuffer, float pan);
    virtual ~SampleSource();
    
    void mixAudio(float* outBuff, int numChannels, int numFrames, bool isLooping = false);
    void setPlayMode(bool isLooping);
    void setPauseMode();
    void setResumeMode();
    void setStopMode();
    bool isPlaying();
    bool isPaused();
    bool isLooping();
    void setPan(float pan);
    float getPan();
    void setGain(float gain);
    float getGain();
    SampleBuffer* buffer();

protected:
    SampleBuffer* _sampleBuffer;
    int32_t _curFrameIndex;
    bool _isPlaying;
    bool _isLooping;
    float _pan;
    float _leftGain;
    float _rightGain;
    float _gain;

private:
    void calcGainFactors();
};

#endif /* IS_ANDROID */
