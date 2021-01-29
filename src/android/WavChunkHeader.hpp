//
//  WavChunkHeader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/22/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "WavTypes.hpp"

class InputStream;

class WavChunkHeader
{
public:
    static const RiffID RIFFID_DATA;

    RiffID _chunkID;
    RiffInt32 _chunkSize;

    WavChunkHeader();
    WavChunkHeader(RiffID chunkID);
    virtual ~WavChunkHeader() {}

    virtual void read(InputStream* inputStream);
};
