//
//  WavFmtChunkHeader.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/22/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_ANDROID

const RiffID WavFmtChunkHeader::RIFFID_FMT = makeRiffID('f', 'm', 't', ' ');
const short WavFmtChunkHeader::ENCODING_PCM = 1;
const short WavFmtChunkHeader::ENCODING_ADPCM = 2;
const short WavFmtChunkHeader::ENCODING_IEEE_FLOAT = 3;

WavFmtChunkHeader::WavFmtChunkHeader() : WavChunkHeader(RIFFID_FMT),
_encodingID(ENCODING_PCM),
_numChannels(0),
_sampleRate(0),
_avgBytesPerSecond(0),
_blockAlign(0),
_sampleSize(0),
_extraBytes(0)
{
    // Empty
}

WavFmtChunkHeader::WavFmtChunkHeader(RiffID tag) : WavChunkHeader(tag),
_encodingID(ENCODING_PCM),
_numChannels(0),
_sampleRate(0),
_avgBytesPerSecond(0),
_blockAlign(0),
_sampleSize(0),
_extraBytes(0)
{
    // Empty
}

void WavFmtChunkHeader::read(InputStream* inputStream)
{
    WavChunkHeader::read(inputStream);
    inputStream->read(&_encodingID, sizeof(_encodingID));
    inputStream->read(&_numChannels, sizeof(_numChannels));
    inputStream->read(&_sampleRate, sizeof(_sampleRate));
    inputStream->read(&_avgBytesPerSecond, sizeof(_avgBytesPerSecond));
    inputStream->read(&_blockAlign, sizeof(_blockAlign));
    inputStream->read(&_sampleSize, sizeof(_sampleSize));

    if (_encodingID != ENCODING_PCM && _encodingID != ENCODING_IEEE_FLOAT)
    {
        inputStream->read(&_extraBytes, sizeof(_extraBytes));
    }
    else
    {
        _extraBytes = (short) (_chunkSize - 16);
    }
}

#endif /* IS_ANDROID */
