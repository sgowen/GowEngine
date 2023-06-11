//
//  SoundLoader.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/30/23.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include "sndfile.h"

#include <AL/alext.h>

enum FormatType
{
    Int16,
    Float,
    IMA4,
    MSADPCM
};

const char* FormatName(ALenum format)
{
    switch(format)
    {
        case AL_FORMAT_MONO8: return "Mono, U8";
        case AL_FORMAT_MONO16: return "Mono, S16";
        case AL_FORMAT_MONO_FLOAT32: return "Mono, Float32";
        case AL_FORMAT_MONO_MULAW: return "Mono, muLaw";
        case AL_FORMAT_MONO_ALAW_EXT: return "Mono, aLaw";
        case AL_FORMAT_MONO_IMA4: return "Mono, IMA4 ADPCM";
        case AL_FORMAT_MONO_MSADPCM_SOFT: return "Mono, MS ADPCM";
        case AL_FORMAT_STEREO8: return "Stereo, U8";
        case AL_FORMAT_STEREO16: return "Stereo, S16";
        case AL_FORMAT_STEREO_FLOAT32: return "Stereo, Float32";
        case AL_FORMAT_STEREO_MULAW: return "Stereo, muLaw";
        case AL_FORMAT_STEREO_ALAW_EXT: return "Stereo, aLaw";
        case AL_FORMAT_STEREO_IMA4: return "Stereo, IMA4 ADPCM";
        case AL_FORMAT_STEREO_MSADPCM_SOFT: return "Stereo, MS ADPCM";
        case AL_FORMAT_QUAD8: return "Quadraphonic, U8";
        case AL_FORMAT_QUAD16: return "Quadraphonic, S16";
        case AL_FORMAT_QUAD32: return "Quadraphonic, Float32";
        case AL_FORMAT_QUAD_MULAW: return "Quadraphonic, muLaw";
        case AL_FORMAT_51CHN8: return "5.1 Surround, U8";
        case AL_FORMAT_51CHN16: return "5.1 Surround, S16";
        case AL_FORMAT_51CHN32: return "5.1 Surround, Float32";
        case AL_FORMAT_51CHN_MULAW: return "5.1 Surround, muLaw";
        case AL_FORMAT_61CHN8: return "6.1 Surround, U8";
        case AL_FORMAT_61CHN16: return "6.1 Surround, S16";
        case AL_FORMAT_61CHN32: return "6.1 Surround, Float32";
        case AL_FORMAT_61CHN_MULAW: return "6.1 Surround, muLaw";
        case AL_FORMAT_71CHN8: return "7.1 Surround, U8";
        case AL_FORMAT_71CHN16: return "7.1 Surround, S16";
        case AL_FORMAT_71CHN32: return "7.1 Surround, Float32";
        case AL_FORMAT_71CHN_MULAW: return "7.1 Surround, muLaw";
        case AL_FORMAT_BFORMAT2D_8: return "B-Format 2D, U8";
        case AL_FORMAT_BFORMAT2D_16: return "B-Format 2D, S16";
        case AL_FORMAT_BFORMAT2D_FLOAT32: return "B-Format 2D, Float32";
        case AL_FORMAT_BFORMAT2D_MULAW: return "B-Format 2D, muLaw";
        case AL_FORMAT_BFORMAT3D_8: return "B-Format 3D, U8";
        case AL_FORMAT_BFORMAT3D_16: return "B-Format 3D, S16";
        case AL_FORMAT_BFORMAT3D_FLOAT32: return "B-Format 3D, Float32";
        case AL_FORMAT_BFORMAT3D_MULAW: return "B-Format 3D, muLaw";
        case AL_FORMAT_UHJ2CHN8_SOFT: return "UHJ 2-channel, U8";
        case AL_FORMAT_UHJ2CHN16_SOFT: return "UHJ 2-channel, S16";
        case AL_FORMAT_UHJ2CHN_FLOAT32_SOFT: return "UHJ 2-channel, Float32";
        case AL_FORMAT_UHJ3CHN8_SOFT: return "UHJ 3-channel, U8";
        case AL_FORMAT_UHJ3CHN16_SOFT: return "UHJ 3-channel, S16";
        case AL_FORMAT_UHJ3CHN_FLOAT32_SOFT: return "UHJ 3-channel, Float32";
        case AL_FORMAT_UHJ4CHN8_SOFT: return "UHJ 4-channel, U8";
        case AL_FORMAT_UHJ4CHN16_SOFT: return "UHJ 4-channel, S16";
        case AL_FORMAT_UHJ4CHN_FLOAT32_SOFT: return "UHJ 4-channel, Float32";
    }
    
    return "Unknown Format";
}

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

ALuint SoundLoader::loadSound(const char *filePath)
{
    enum FormatType sample_format = Int16;
    ALint byteblockalign = 0;
    ALint splblockalign = 0;
    sf_count_t num_frames;
    ALenum err, format;
    ALsizei num_bytes;
    SNDFILE* sndfile;
    SF_INFO sfinfo;
    ALuint buffer;
    void* membuf;

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
        fprintf(stderr, "Could not open audio in %s: %s\n", filePath, sf_strerror(sndfile));
        return 0;
    }
    
    if (sfinfo.frames < 1)
    {
        fprintf(stderr, "Bad sample count in %s (%" PRId64 ")\n", filePath, sfinfo.frames);
        sf_close(sndfile);
        ASSET_HANDLER.unloadAsset(soundData);
        return 0;
    }

    switch ((sfinfo.format&SF_FORMAT_SUBMASK))
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
            if (alIsExtensionPresent("AL_EXT_FLOAT32"))
            {
                sample_format = Float;
            }
            break;
        case SF_FORMAT_IMA_ADPCM:
            if (sfinfo.channels <= 2 && (sfinfo.format&SF_FORMAT_TYPEMASK) == SF_FORMAT_WAV
                && alIsExtensionPresent("AL_EXT_IMA4")
                && alIsExtensionPresent("AL_SOFT_block_alignment"))
            {
                sample_format = IMA4;
            }
            break;
        case SF_FORMAT_MS_ADPCM:
            if(sfinfo.channels <= 2 && (sfinfo.format&SF_FORMAT_TYPEMASK) == SF_FORMAT_WAV
                && alIsExtensionPresent("AL_SOFT_MSADPCM")
                && alIsExtensionPresent("AL_SOFT_block_alignment"))
            {
                sample_format = MSADPCM;
            }
            break;
    }

    if (sample_format == IMA4 || sample_format == MSADPCM)
    {
        SF_CHUNK_INFO inf = { "fmt ", 4, 0, nullptr };
        SF_CHUNK_ITERATOR *iter = sf_get_chunk_iterator(sndfile, &inf);

        if (!iter || sf_get_chunk_size(iter, &inf) != SF_ERR_NO_ERROR || inf.datalen < 14)
        {
            sample_format = Int16;
        }
        else
        {
            ALubyte *fmtbuf = static_cast<ALubyte*>(calloc(inf.datalen, 1));
            inf.data = fmtbuf;
            if (sf_get_chunk_data(iter, &inf) != SF_ERR_NO_ERROR)
            {
                sample_format = Int16;
            }
            else
            {
                byteblockalign = fmtbuf[12] | (fmtbuf[13]<<8);
                if (sample_format == IMA4)
                {
                    splblockalign = (byteblockalign/sfinfo.channels - 4)/4*8 + 1;
                    if (splblockalign < 1
                        || ((splblockalign-1)/2 + 4)*sfinfo.channels != byteblockalign)
                    {
                        sample_format = Int16;
                    }
                }
                else
                {
                    splblockalign = (byteblockalign/sfinfo.channels - 7)*2 + 2;
                    if (splblockalign < 2
                        || ((splblockalign-2)/2 + 7)*sfinfo.channels != byteblockalign)
                    {
                        sample_format = Int16;
                    }
                }
            }
            free(fmtbuf);
        }
    }

    if (sample_format == Int16)
    {
        splblockalign = 1;
        byteblockalign = sfinfo.channels * 2;
    }
    else if (sample_format == Float)
    {
        splblockalign = 1;
        byteblockalign = sfinfo.channels * 4;
    }

    format = AL_NONE;
    if (sfinfo.channels == 1)
    {
        if (sample_format == Int16)
        {
            format = AL_FORMAT_MONO16;
        }
        else if(sample_format == Float)
        {
            format = AL_FORMAT_MONO_FLOAT32;
        }
        else if(sample_format == IMA4)
        {
            format = AL_FORMAT_MONO_IMA4;
        }
        else if(sample_format == MSADPCM)
        {
            format = AL_FORMAT_MONO_MSADPCM_SOFT;
        }
    }
    else if (sfinfo.channels == 2)
    {
        if (sample_format == Int16)
        {
            format = AL_FORMAT_STEREO16;
        }
        else if (sample_format == Float)
        {
            format = AL_FORMAT_STEREO_FLOAT32;
        }
        else if (sample_format == IMA4)
        {
            format = AL_FORMAT_STEREO_IMA4;
        }
        else if (sample_format == MSADPCM)
        {
            format = AL_FORMAT_STEREO_MSADPCM_SOFT;
        }
    }
    else if (sfinfo.channels == 3)
    {
        if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, nullptr, 0) == SF_AMBISONIC_B_FORMAT)
        {
            if (sample_format == Int16)
            {
                format = AL_FORMAT_BFORMAT2D_16;
            }
            else if(sample_format == Float)
            {
                format = AL_FORMAT_BFORMAT2D_FLOAT32;
            }
        }
    }
    else if(sfinfo.channels == 4)
    {
        if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, nullptr, 0) == SF_AMBISONIC_B_FORMAT)
        {
            if (sample_format == Int16)
            {
                format = AL_FORMAT_BFORMAT3D_16;
            }
            else if(sample_format == Float)
            {
                format = AL_FORMAT_BFORMAT3D_FLOAT32;
            }
        }
    }
    
    if (!format)
    {
        fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
        sf_close(sndfile);
        ASSET_HANDLER.unloadAsset(soundData);
        return 0;
    }

    if (sfinfo.frames/splblockalign > (sf_count_t)(INT_MAX/byteblockalign))
    {
        fprintf(stderr, "Too many samples in %s (%" PRId64 ")\n", filePath, sfinfo.frames);
        sf_close(sndfile);
        ASSET_HANDLER.unloadAsset(soundData);
        return 0;
    }

    membuf = malloc((size_t)(sfinfo.frames / splblockalign * byteblockalign));

    if (sample_format == Int16)
    {
        num_frames = sf_readf_short(sndfile, static_cast<short*>(membuf), sfinfo.frames);
    }
    else if(sample_format == Float)
    {
        num_frames = sf_readf_float(sndfile, static_cast<float*>(membuf), sfinfo.frames);
    }
    else
    {
        sf_count_t count = sfinfo.frames / splblockalign * byteblockalign;
        num_frames = sf_read_raw(sndfile, membuf, count);
        if (num_frames > 0)
        {
            num_frames = num_frames / byteblockalign * splblockalign;
        }
    }
    
    if (num_frames < 1)
    {
        free(membuf);
        sf_close(sndfile);
        ASSET_HANDLER.unloadAsset(soundData);
        fprintf(stderr, "Failed to read samples in %s (%" PRId64 ")\n", filePath, num_frames);
        return 0;
    }
    num_bytes = (ALsizei)(num_frames / splblockalign * byteblockalign);

    printf("Loading: %s (%s, %dhz)\n", filePath, FormatName(format), sfinfo.samplerate);
    fflush(stdout);

    // TODO consider moving the below into an OpenALUtil class.
    
    buffer = 0;
    alGenBuffers(1, &buffer);
    if (splblockalign > 1)
    {
        alBufferi(buffer, AL_UNPACK_BLOCK_ALIGNMENT_SOFT, splblockalign);
    }
    alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

    // TODO consider moving the below into an another method to be called from the SoundManager after invoking OpenALUtil
    
    free(membuf);
    sf_close(sndfile);
    ASSET_HANDLER.unloadAsset(soundData);

    err = alGetError();
    if (err != AL_NO_ERROR)
    {
        fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
        if (buffer && alIsBuffer(buffer))
        {
            alDeleteBuffers(1, &buffer);
        }
        return 0;
    }

    return buffer;
}
