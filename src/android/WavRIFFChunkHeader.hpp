//
//  WavRIFFChunkHeader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/22/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "WavChunkHeader.hpp"

class WavRIFFChunkHeader : public WavChunkHeader
{
public:
    static const RiffID RIFFID_RIFF;
    static const RiffID RIFFID_WAVE;

    RiffID _formatID;

    WavRIFFChunkHeader();
    WavRIFFChunkHeader(RiffID tag);
    virtual ~WavRIFFChunkHeader();

    virtual void read(InputStream* inputStream);
};
