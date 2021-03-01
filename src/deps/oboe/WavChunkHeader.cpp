//
//  WavChunkHeader.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/22/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "PlatformMacros.hpp"
#if IS_ANDROID

#include "WavChunkHeader.hpp"

#include "InputStream.hpp"

const RiffID WavChunkHeader::RIFFID_DATA = makeRiffID('d', 'a', 't', 'a');

WavChunkHeader::WavChunkHeader() :
_chunkID(0),
_chunkSize(0)
{
    // Empty
}

WavChunkHeader::WavChunkHeader(RiffID chunkID) :
_chunkID(chunkID),
_chunkSize(0)
{
    // Empty
}

void WavChunkHeader::read(InputStream* inputStream)
{
    inputStream->read(&_chunkID, sizeof(_chunkID));
    inputStream->read(&_chunkSize, sizeof(_chunkSize));
}

#endif /* IS_ANDROID */
