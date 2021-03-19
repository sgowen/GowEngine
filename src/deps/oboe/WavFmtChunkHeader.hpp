//
//  WavFmtChunkHeader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/22/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/PlatformMacros.hpp"

#if IS_ANDROID

#include "WavChunkHeader.hpp"

class WavFmtChunkHeader : public WavChunkHeader
{
public:
    static const RiffID RIFFID_FMT;
    static const short ENCODING_PCM;
    static const short ENCODING_ADPCM;
    static const short ENCODING_IEEE_FLOAT;

    RiffInt16 _encodingID;
    RiffInt16 _numChannels;
    RiffInt32 _sampleRate;
    RiffInt32 _avgBytesPerSecond;
    RiffInt16 _blockAlign;
    RiffInt16 _sampleSize;
    RiffInt16 _extraBytes;

    WavFmtChunkHeader();
    WavFmtChunkHeader(RiffID tag);
    virtual ~WavFmtChunkHeader() {}

    virtual void read(InputStream* inputStream);
};

#endif /* IS_ANDROID */
