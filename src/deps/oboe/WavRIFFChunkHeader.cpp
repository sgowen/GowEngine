//
//  WavRIFFChunkHeader.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/22/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "PlatformMacros.hpp"
#if IS_ANDROID

#include "WavRIFFChunkHeader.hpp"

#include "InputStream.hpp"

const RiffID WavRIFFChunkHeader::RIFFID_RIFF = makeRiffID('R', 'I', 'F', 'F');
const RiffID WavRIFFChunkHeader::RIFFID_WAVE = makeRiffID('W', 'A', 'V', 'E');

WavRIFFChunkHeader::WavRIFFChunkHeader() : WavChunkHeader(RIFFID_RIFF),
_formatID(RIFFID_WAVE)
{
    // Empty
}

WavRIFFChunkHeader::WavRIFFChunkHeader(RiffID tag) : WavChunkHeader(tag),
_formatID(RIFFID_WAVE)
{
    // Empty
}

void WavRIFFChunkHeader::read(InputStream* inputStream)
{
    WavChunkHeader::read(inputStream);
    inputStream->read(&_formatID, sizeof(_formatID));
}

#endif /* IS_ANDROID */
