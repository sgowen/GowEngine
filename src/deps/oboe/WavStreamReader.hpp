//
//  WavStreamReader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/22/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/PlatformMacros.hpp"
#if IS_ANDROID

#include "WavTypes.hpp"

#include <map>

class InputStream;
class WavRIFFChunkHeader;
class WavFmtChunkHeader;
class WavChunkHeader;

class WavStreamReader
{
public:
    WavStreamReader(InputStream* inputStream);
    ~WavStreamReader();

    void parse();
    void positionToAudio();
    int getDataFloat(float *buff, int numFrames);
    int getSampleRate();
    int getNumSampleFrames();
    int getNumChannels();
    int getBitsPerSample();

protected:
    InputStream *_inputStream;
    WavRIFFChunkHeader* _wavChunk;
    WavFmtChunkHeader* _fmtChunk;
    WavChunkHeader* _dataChunk;
    long _audioDataStartPos;
    std::map<RiffID, WavChunkHeader *>* _chunkMap;
};

#endif /* IS_ANDROID */
