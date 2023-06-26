//
//  Sound.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/24/23.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/assets/SoundDescriptor.hpp"

enum FormatType
{
    Int16,
    Float,
    IMA4,
    MSADPCM
};

#define SOUND_FORMAT_NONE                0
#define SOUND_FORMAT_MONO16              0x1101
#define SOUND_FORMAT_MONO_FLOAT32        0x10010
#define SOUND_FORMAT_MONO_IMA4           0x1300
#define SOUND_FORMAT_MONO_MSADPCM_SOFT   0x1302
#define SOUND_FORMAT_STEREO16            0x1103
#define SOUND_FORMAT_STEREO_FLOAT32      0x10011
#define SOUND_FORMAT_STEREO_IMA4         0x1301
#define SOUND_FORMAT_STEREO_MSADPCM_SOFT 0x1303
#define SOUND_FORMAT_BFORMAT2D_16        0x20022
#define SOUND_FORMAT_BFORMAT2D_FLOAT32   0x20023
#define SOUND_FORMAT_BFORMAT3D_16        0x20032
#define SOUND_FORMAT_BFORMAT3D_FLOAT32   0x20033

struct Sound
{
    SoundDescriptor _desc;
    FormatType _sampleFormat;
    int32_t _splblockalign;
    int32_t _format;
    void* _data;
    int32_t _numBytes;
    int32_t _sampleRate;
    uint32_t* _alHandles;
    uint8_t _sourceIndex;
    
    Sound(SoundDescriptor desc) :
    _desc(desc),
    _sampleFormat(Int16),
    _splblockalign(0),
    _format(SOUND_FORMAT_NONE),
    _data(nullptr),
    _numBytes(0),
    _sampleRate(0),
    _alHandles(nullptr),
    _sourceIndex(0)
    {
        // Empty
    }
    
    uint32_t nextSource()
    {
        ++_sourceIndex;
        if (_sourceIndex >= _desc._numInstances)
        {
            _sourceIndex = 0;
        }
        
        return _alHandles[_sourceIndex];
    }
};
