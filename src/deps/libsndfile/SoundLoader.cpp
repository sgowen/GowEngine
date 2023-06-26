//
//  SoundLoader.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/30/23.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include "sndfile.h"

struct gowengine_sf_file
{
    char *buffer;
    sf_count_t length;
    sf_count_t curpos;
};

static sf_count_t gowengine_sf_vio_get_filelen (void *user_data)
{
    struct gowengine_sf_file *file = (struct gowengine_sf_file*) user_data;
    return file->length;
}

static sf_count_t gowengine_sf_vio_read (void *ptr, sf_count_t count, void *user_data)
{
    struct gowengine_sf_file *file = (struct gowengine_sf_file*) user_data;
    if (count + file->curpos > file->length)
    {
        count = file->length - file->curpos;
    }
    if (count > 0)
    {
        memcpy(ptr, file->buffer + file->curpos, count);
    }
    file->curpos += count;
    return count;
}

static sf_count_t gowengine_sf_vio_write (const void *ptr, sf_count_t count, void *user_data)
{
    return 0;
}

static sf_count_t gowengine_sf_vio_seek (sf_count_t offset, int whence, void *user_data)
{
    struct gowengine_sf_file* file = (struct gowengine_sf_file*) user_data;
    
    sf_count_t newpos = 0;
    switch (whence)
    {
        case SEEK_SET:
            newpos = offset;
            break;
        case SEEK_CUR:
            newpos = file->curpos + offset;
            break;
        case SEEK_END:
            newpos = file->length + offset;
            break;
    }
    
    if (newpos >= 0 && newpos < file->length)
    {
        file->curpos = newpos;
    }
    else
    {
        return 1;
    }
    
    return 0;
}

static sf_count_t gowengine_sf_vio_tell (void* user_data)
{
    struct gowengine_sf_file* file = (struct gowengine_sf_file*) user_data;
    return file->curpos;
}

void SoundLoader::loadData(Sound& s)
{
    const char* filePath = s._desc._filePath.c_str();
    if (ENGINE_CFG.fileLoggingEnabled())
    {
        LOG("SoundLoader::loadData %s", filePath);
    }
    
    int32_t byteblockalign = 0;
    sf_count_t num_frames;
    SNDFILE* sndfile;
    SF_INFO sfinfo;

    SF_VIRTUAL_IO io;
    io.get_filelen = gowengine_sf_vio_get_filelen;
    io.seek = gowengine_sf_vio_seek;
    io.read = gowengine_sf_vio_read;
    io.write = gowengine_sf_vio_write;
    io.tell = gowengine_sf_vio_tell;

    FileData soundData = ASSET_HANDLER.loadAsset(filePath);
    
    struct gowengine_sf_file file;
    file.buffer = (char*) soundData._data;
    file.length = soundData._length;
    file.curpos = 0;
    
    sndfile = sf_open_virtual (&io, SFM_READ, &sfinfo, &file);
    if (!sndfile)
    {
        LOG("Could not open audio in %s: %s", filePath, sf_strerror(sndfile));
    }
    assert(sndfile != nullptr);
    
    if (sfinfo.frames < 1)
    {
        LOG("Bad sample count in %s (%" PRId64 ")", filePath, sfinfo.frames);
        sf_close(sndfile);
        ASSET_HANDLER.unloadAsset(soundData);
    }
    assert(sfinfo.frames > 0);

    int formatSubmask = sfinfo.format & SF_FORMAT_SUBMASK;
    switch (formatSubmask)
    {
        case SF_FORMAT_PCM_24:
        case SF_FORMAT_PCM_32:
        case SF_FORMAT_FLOAT:
        case SF_FORMAT_DOUBLE:
        case SF_FORMAT_VORBIS:
        case SF_FORMAT_OPUS:
        case SF_FORMAT_ALAC_20:
        case SF_FORMAT_ALAC_24:
        case SF_FORMAT_ALAC_32:
        case SF_FORMAT_MPEG_LAYER_I:
        case SF_FORMAT_MPEG_LAYER_II:
        case SF_FORMAT_MPEG_LAYER_III:
            s._sampleFormat = Float;
            break;
        case SF_FORMAT_IMA_ADPCM:
            if (sfinfo.channels <= 2 && (sfinfo.format&SF_FORMAT_TYPEMASK) == SF_FORMAT_WAV)
            {
                s._sampleFormat = IMA4;
            }
            break;
        case SF_FORMAT_MS_ADPCM:
            if (sfinfo.channels <= 2 && (sfinfo.format&SF_FORMAT_TYPEMASK) == SF_FORMAT_WAV)
            {
                s._sampleFormat = MSADPCM;
            }
            break;
    }

    if (s._sampleFormat == IMA4 || s._sampleFormat == MSADPCM)
    {
        SF_CHUNK_INFO inf = { "fmt ", 4, 0, nullptr };
        SF_CHUNK_ITERATOR *iter = sf_get_chunk_iterator(sndfile, &inf);

        if (!iter || sf_get_chunk_size(iter, &inf) != SF_ERR_NO_ERROR || inf.datalen < 14)
        {
            s._sampleFormat = Int16;
        }
        else
        {
            unsigned char *fmtbuf = static_cast<unsigned char*>(calloc(inf.datalen, 1));
            inf.data = fmtbuf;
            if (sf_get_chunk_data(iter, &inf) != SF_ERR_NO_ERROR)
            {
                s._sampleFormat = Int16;
            }
            else
            {
                byteblockalign = fmtbuf[12] | (fmtbuf[13]<<8);
                if (s._sampleFormat == IMA4)
                {
                    s._splblockalign = (byteblockalign/sfinfo.channels - 4)/4*8 + 1;
                    if (s._splblockalign < 1
                        || ((s._splblockalign-1)/2 + 4)*sfinfo.channels != byteblockalign)
                    {
                        s._sampleFormat = Int16;
                    }
                }
                else
                {
                    s._splblockalign = (byteblockalign/sfinfo.channels - 7)*2 + 2;
                    if (s._splblockalign < 2
                        || ((s._splblockalign-2)/2 + 7)*sfinfo.channels != byteblockalign)
                    {
                        s._sampleFormat = Int16;
                    }
                }
            }
            free(fmtbuf);
        }
    }

    if (s._sampleFormat == Int16)
    {
        s._splblockalign = 1;
        byteblockalign = sfinfo.channels * 2;
    }
    else if (s._sampleFormat == Float)
    {
        s._splblockalign = 1;
        byteblockalign = sfinfo.channels * 4;
    }

    s._format = SOUND_FORMAT_NONE;
    if (sfinfo.channels == 1)
    {
        if (s._sampleFormat == Int16)
        {
            s._format = SOUND_FORMAT_MONO16;
        }
        else if (s._sampleFormat == Float)
        {
            s._format = SOUND_FORMAT_MONO_FLOAT32;
        }
        else if (s._sampleFormat == IMA4)
        {
            s._format = SOUND_FORMAT_MONO_IMA4;
        }
        else if (s._sampleFormat == MSADPCM)
        {
            s._format = SOUND_FORMAT_MONO_MSADPCM_SOFT;
        }
    }
    else if (sfinfo.channels == 2)
    {
        if (s._sampleFormat == Int16)
        {
            s._format = SOUND_FORMAT_STEREO16;
        }
        else if (s._sampleFormat == Float)
        {
            s._format = SOUND_FORMAT_STEREO_FLOAT32;
        }
        else if (s._sampleFormat == IMA4)
        {
            s._format = SOUND_FORMAT_STEREO_IMA4;
        }
        else if (s._sampleFormat == MSADPCM)
        {
            s._format = SOUND_FORMAT_STEREO_MSADPCM_SOFT;
        }
    }
    else if (sfinfo.channels == 3)
    {
        if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, nullptr, 0) == SF_AMBISONIC_B_FORMAT)
        {
            if (s._sampleFormat == Int16)
            {
                s._format = SOUND_FORMAT_BFORMAT2D_16;
            }
            else if (s._sampleFormat == Float)
            {
                s._format = SOUND_FORMAT_BFORMAT2D_FLOAT32;
            }
        }
    }
    else if (sfinfo.channels == 4)
    {
        if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, nullptr, 0) == SF_AMBISONIC_B_FORMAT)
        {
            if (s._sampleFormat == Int16)
            {
                s._format = SOUND_FORMAT_BFORMAT3D_16;
            }
            else if (s._sampleFormat == Float)
            {
                s._format = SOUND_FORMAT_BFORMAT3D_FLOAT32;
            }
        }
    }
    
    if (s._format == SOUND_FORMAT_NONE)
    {
        LOG("Unsupported channel count: %d", sfinfo.channels);
        sf_close(sndfile);
        ASSET_HANDLER.unloadAsset(soundData);
    }
    assert(s._format != SOUND_FORMAT_NONE);

    bool areSamplesInvalid = (sfinfo.frames / s._splblockalign) > (sf_count_t)(INT_MAX / byteblockalign);
    if (areSamplesInvalid)
    {
        LOG("Too many samples in %s (%" PRId64 ")", filePath, sfinfo.frames);
        sf_close(sndfile);
        ASSET_HANDLER.unloadAsset(soundData);
    }
    assert(areSamplesInvalid == false);

    s._data = malloc((size_t)(sfinfo.frames / s._splblockalign * byteblockalign));

    if (s._sampleFormat == Int16)
    {
        num_frames = sf_readf_short(sndfile, static_cast<short*>(s._data), sfinfo.frames);
    }
    else if (s._sampleFormat == Float)
    {
        num_frames = sf_readf_float(sndfile, static_cast<float*>(s._data), sfinfo.frames);
    }
    else
    {
        sf_count_t count = sfinfo.frames / s._splblockalign * byteblockalign;
        num_frames = sf_read_raw(sndfile, s._data, count);
        if (num_frames > 0)
        {
            num_frames = num_frames / byteblockalign * s._splblockalign;
        }
    }
    
    sf_close(sndfile);
    ASSET_HANDLER.unloadAsset(soundData);
    
    if (num_frames < 1)
    {
        LOG("Failed to read samples in %s (%" PRId64 ")", filePath, num_frames);
    }
    assert(num_frames > 0);
    
    s._numBytes = (uint32_t)(num_frames / s._splblockalign * byteblockalign);
    s._sampleRate = sfinfo.samplerate;
}

void SoundLoader::freeData(Sound& s)
{
    if (ENGINE_CFG.fileLoggingEnabled())
    {
        LOG("SoundLoader::freeData %s", s._desc._filePath.c_str());
    }
    
    assert(s._data != nullptr);
    
    free(s._data);
    s._data = nullptr;
}
