//
//  WavStreamReader.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/22/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "WavStreamReader.hpp"

#include "InputStream.hpp"
#include "WavRIFFChunkHeader.hpp"
#include "WavFmtChunkHeader.hpp"
#include "WavChunkHeader.hpp"
#include "GowUtil.hpp"

#include <algorithm>
#include <string.h>

#include <android/log.h>

static const char *TAG = "WavStreamReader";

WavStreamReader::WavStreamReader(InputStream* inputStream) :
_inputStream(inputStream),
_wavChunk(0),
_fmtChunk(0),
_dataChunk(0),
_audioDataStartPos(-1),
_chunkMap(new std::map<RiffID, WavChunkHeader *>())
{
    // Empty
}

WavStreamReader::~WavStreamReader()
{
    GowUtil::cleanUpMapOfPointers(*_chunkMap);
    delete _chunkMap;
}

void WavStreamReader::parse()
{
    RiffID tag;

    while (true)
    {
        int numRead = _inputStream->peek(&tag, sizeof(tag));
        if (numRead <= 0)
        {
            break;
        }

        char *tagStr = (char *) &tag;
        __android_log_print(ANDROID_LOG_INFO, TAG, "[%c%c%c%c]", tagStr[0], tagStr[1], tagStr[2], tagStr[3]);

        WavChunkHeader *chunk = 0;
        if (tag == WavRIFFChunkHeader::RIFFID_RIFF)
        {
            chunk = _wavChunk = new WavRIFFChunkHeader(tag);
            _wavChunk->read(_inputStream);
        }
        else if (tag == WavFmtChunkHeader::RIFFID_FMT)
        {
            chunk = _fmtChunk = new WavFmtChunkHeader(tag);
            _fmtChunk->read(_inputStream);
        }
        else if (tag == WavChunkHeader::RIFFID_DATA)
        {
            chunk = _dataChunk = new WavChunkHeader(tag);
            _dataChunk->read(_inputStream);
            // We are now positioned at the start of the audio data.
            _audioDataStartPos = _inputStream->getPos();
            _inputStream->advance(_dataChunk->_chunkSize);
        }
        else
        {
            chunk = new WavChunkHeader(tag);
            chunk->read(_inputStream);
            _inputStream->advance(chunk->_chunkSize); // skip the body
        }

        (*_chunkMap)[tag] = chunk;
    }

    if (_dataChunk != 0)
    {
        _inputStream->setPos(_audioDataStartPos);
    }
}

void WavStreamReader::positionToAudio()
{
    if (_dataChunk != 0)
    {
        _inputStream->setPos(_audioDataStartPos);
    }
}

int WavStreamReader::getDataFloat(float *buff, int numFrames)
{
    __android_log_print(ANDROID_LOG_INFO, TAG, "getData(%d)", numFrames);

    if (_dataChunk == 0 || _fmtChunk == 0)
    {
        return 0;
    }

    int totalFramesRead = 0;

    int numChans = _fmtChunk->_numChannels;
    int buffOffset = 0;

    if (_fmtChunk->_sampleSize != 16)
    {
        return 0;
    }

    short *readBuff = new short[128 * numChans];
    int framesLeft = numFrames;
    while (framesLeft > 0)
    {
        int framesThisRead = std::min(framesLeft, 128);
        __android_log_print(ANDROID_LOG_INFO, TAG, "read(%d)", framesThisRead);
        int numFramesRead = _inputStream->read(readBuff, framesThisRead * sizeof(short) * numChans) / (sizeof(short) * numChans);
        totalFramesRead += numFramesRead;

        for (int offset = 0; offset < numFramesRead * numChans; offset++)
        {
            buff[buffOffset++] = (float) readBuff[offset] / (float) 0x7FFF;
        }

        if (numFramesRead < framesThisRead)
        {
            break;
        }

        framesLeft -= framesThisRead;
    }
    delete[] readBuff;

    __android_log_print(ANDROID_LOG_INFO, TAG, "  returns:%d", totalFramesRead);
    
    return totalFramesRead;
}

int WavStreamReader::getSampleRate()
{
    return _fmtChunk->_sampleRate;
}

int WavStreamReader::getNumSampleFrames()
{
    return _dataChunk->_chunkSize / (_fmtChunk->_sampleSize / 8) / _fmtChunk->_numChannels;
}

int WavStreamReader::getNumChannels()
{
    return _fmtChunk != 0 ? _fmtChunk->_numChannels : 0;
}

int WavStreamReader::getBitsPerSample()
{
    return _fmtChunk->_sampleSize;
}
