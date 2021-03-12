//
//  SampleSource.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/22/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>
#if IS_ANDROID

SampleSource::SampleSource(SampleBuffer* sampleBuffer, float pan) :
_sampleBuffer(sampleBuffer),
_curFrameIndex(0),
_isPlaying(false),
_isLooping(false),
_gain(1.0f)
{
    setPan(pan);
}

SampleSource::~SampleSource()
{
    delete _sampleBuffer;
}

void SampleSource::mixAudio(float* outBuff, int numChannels, int numFrames, bool isLooping)
{
    int32_t numSampleFrames = _sampleBuffer->getNumSampleFrames();
    int32_t numWriteFrames = _isPlaying ? std::min(numFrames, numSampleFrames - _curFrameIndex) : 0;

    if (numWriteFrames != 0)
    {
        const float* data  = _sampleBuffer->getSampleData();
        if (numChannels == 1)
        {
            for (int32_t frameIndex = 0; frameIndex < numWriteFrames; frameIndex++)
            {
                outBuff[frameIndex] += data[_curFrameIndex++] * _gain;
            }
        }
        else if (numChannels == 2)
        {
            int dstSampleIndex = 0;
            for (int32_t frameIndex = 0; frameIndex < numWriteFrames; frameIndex++)
            {
                outBuff[dstSampleIndex++] += data[_curFrameIndex] * _leftGain;
                outBuff[dstSampleIndex++] += data[_curFrameIndex++] * _rightGain;
            }
        }

        if (_curFrameIndex >= numSampleFrames)
        {
            if (isLooping)
            {
                _curFrameIndex = 0;
            }
            else
            {
                _isPlaying = false;
            }
        }
    }
}

void SampleSource::setPlayMode(bool isLooping)
{
    _isLooping = isLooping;
    _curFrameIndex = 0;
    _isPlaying = true;
}

void SampleSource::setPauseMode()
{
    _isPlaying = false;
}

void SampleSource::setResumeMode()
{
    _isPlaying = true;
}

void SampleSource::setStopMode()
{
    _isLooping = false;
    _isPlaying = false;
    _curFrameIndex = 0;
}

bool SampleSource::isPlaying()
{
    return _isPlaying;
}

bool SampleSource::isPaused()
{
    return !_isPlaying && _curFrameIndex > 0;
}

bool SampleSource::isLooping()
{
    return _isLooping;
}

void SampleSource::setPan(float pan)
{
    if (pan < PAN_HARDLEFT)
    {
        _pan = PAN_HARDLEFT;
    }
    else if (pan > PAN_HARDRIGHT)
    {
        _pan = PAN_HARDRIGHT;
    }
    else
    {
        _pan = pan;
    }
    calcGainFactors();
}

float SampleSource::getPan()
{
    return _pan;
}

void SampleSource::setGain(float gain)
{
    _gain = gain;
    calcGainFactors();
}

float SampleSource::getGain()
{
    return _gain;
}

SampleBuffer* SampleSource::buffer()
{
    return _sampleBuffer;
}

void SampleSource::calcGainFactors()
{
    float rightPan = (_pan * 0.5) + 0.5;
    _rightGain = rightPan * _gain;
    _leftGain = (1.0 - rightPan) * _gain;
}

#endif /* IS_ANDROID */
