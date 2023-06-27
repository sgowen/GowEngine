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
    FormatType_Int16,
    FormatType_Float,
    FormatType_IMA4,
    FormatType_MSADPCM
};

enum Format
{
    Format_NONE,
    Format_MONO16,
    Format_MONO_FLOAT32,
    Format_MONO_IMA4,
    Format_MONO_MSADPCM_SOFT,
    Format_STEREO16,
    Format_STEREO_FLOAT32,
    Format_STEREO_IMA4,
    Format_STEREO_MSADPCM_SOFT,
    Format_BFORMAT2D_16,
    Format_BFORMAT2D_FLOAT32,
    Format_BFORMAT3D_16,
    Format_BFORMAT3D_FLOAT32
};

struct Sound
{
    SoundDescriptor _desc;
    FormatType _formatType;
    int32_t _splblockalign;
    Format _format;
    void* _data;
    int32_t _numBytes;
    int32_t _sampleRate;
    uint32_t* _alHandles;
    uint8_t _sourceIndex;
    
    Sound(SoundDescriptor desc) :
    _desc(desc),
    _formatType(FormatType_Int16),
    _splblockalign(0),
    _format(Format_NONE),
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
