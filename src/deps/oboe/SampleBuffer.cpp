//
//  SampleBuffer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/22/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_ANDROID

#include "resampler/MultiChannelResampler.h"

SampleBuffer::SampleBuffer() :
_sampleData(nullptr),
_numSamples(0)
{
    // Empty
}

SampleBuffer::~SampleBuffer()
{
    unloadSampleData();
}

void SampleBuffer::loadSampleData(WavStreamReader* reader)
{
    _audioProperties._channelCount = reader->getNumChannels();
    _audioProperties._sampleRate = reader->getSampleRate();

    reader->positionToAudio();

    _numSamples = reader->getNumSampleFrames() * reader->getNumChannels();
    _sampleData = new float[_numSamples];

    reader->getDataFloat(_sampleData, reader->getNumSampleFrames());
}

void SampleBuffer::unloadSampleData()
{
    if (_sampleData != nullptrptr)
    {
        delete[] _sampleData;
        _sampleData = nullptrptr;
    }
    _numSamples = 0;
}

class ResampleBlock
{
public:
    int32_t _sampleRate;
    float*  _buffer;
    int32_t _numFrames;
};

void _resampleData(const ResampleBlock& input, ResampleBlock* output, int numChannels)
{
    using namespace resampler;
    
    double temp =
            ((double)input._numFrames * (double)output->_sampleRate) / (double)input._sampleRate;

    int32_t numOutFrames = (int32_t)(temp + 0.5);
    numOutFrames += 8;

    MultiChannelResampler *resampler = MultiChannelResampler::make(
            numChannels,
            input._sampleRate,
            output->_sampleRate,
            MultiChannelResampler::Quality::Medium);

    float *inputBuffer = input._buffer;
    float *outputBuffer = new float[numOutFrames];
    output->_buffer = outputBuffer;

    int numOutputFrames = 0;
    int inputFramesLeft = input._numFrames;
    while (inputFramesLeft > 0)
    {
        if (resampler->isWriteNeeded())
        {
            resampler->writeNextFrame(inputBuffer);
            inputBuffer += numChannels;
            inputFramesLeft--;
        }
        else
        {
            resampler->readNextFrame(outputBuffer);
            outputBuffer += numChannels;
            numOutputFrames++;
        }
    }
    output->_numFrames = numOutputFrames;

    delete resampler;
}

void SampleBuffer::resampleData(int sampleRate)
{
    if (_audioProperties._sampleRate == sampleRate)
    {
        return;
    }

    ResampleBlock inputBlock;
    inputBlock._buffer = _sampleData;
    inputBlock._numFrames = _numSamples;
    inputBlock._sampleRate = _audioProperties._sampleRate;

    ResampleBlock outputBlock;
    outputBlock._sampleRate = sampleRate;
    _resampleData(inputBlock, &outputBlock, _audioProperties._channelCount);

    delete[] _sampleData;

    _sampleData = outputBlock._buffer;
    _numSamples = outputBlock._numFrames;
    _audioProperties._sampleRate = outputBlock._sampleRate;
}

AudioProperties SampleBuffer::getProperties() const
{
    return _audioProperties;
}

float* SampleBuffer::getSampleData()
{
    return _sampleData;
}

int32_t SampleBuffer::getNumSampleFrames()
{
    return _numSamples;
}

#endif /* IS_ANDROID */
