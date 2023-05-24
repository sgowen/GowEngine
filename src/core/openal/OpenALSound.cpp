//
//  OpenALSound.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_LINUX || IS_APPLE

#include "sndfile.h"

#include "AL/al.h"
#include "AL/alext.h"

#include "alhelpers.h"

enum FormatType {
    Int16,
    Float,
    IMA4,
    MSADPCM
};

/* LoadBuffer loads the named audio file into an OpenAL buffer object, and
 * returns the new buffer ID.
 */
static ALuint LoadSound(const char *filename)
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

    /* Open the audio file and check that it's usable. */
    sndfile = sf_open(filename, SFM_READ, &sfinfo);
    if (!sndfile)
    {
        fprintf(stderr, "Could not open audio in %s: %s\n", filename, sf_strerror(sndfile));
        return 0;
    }
    
    if (sfinfo.frames < 1)
    {
        fprintf(stderr, "Bad sample count in %s (%" PRId64 ")\n", filename, sfinfo.frames);
        sf_close(sndfile);
        return 0;
    }

    /* Detect a suitable format to load. Formats like Vorbis and Opus use float
     * natively, so load as float to avoid clipping when possible. Formats
     * larger than 16-bit can also use float to preserve a bit more precision.
     */
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
        case 0x0080/*SF_FORMAT_MPEG_LAYER_I*/:
        case 0x0081/*SF_FORMAT_MPEG_LAYER_II*/:
        case 0x0082/*SF_FORMAT_MPEG_LAYER_III*/:
            if (alIsExtensionPresent("AL_EXT_FLOAT32"))
            {
                sample_format = Float;
            }
            break;
        case SF_FORMAT_IMA_ADPCM:
            /* ADPCM formats require setting a block alignment as specified in the
             * file, which needs to be read from the wave 'fmt ' chunk manually
             * since libsndfile doesn't provide it in a format-agnostic way.
             */
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
                sample_format = MSADPCM;
            break;
    }

    if (sample_format == IMA4 || sample_format == MSADPCM)
    {
        /* For ADPCM, lookup the wave file's "fmt " chunk, which is a
         * WAVEFORMATEX-based structure for the audio format.
         */
        SF_CHUNK_INFO inf = { "fmt ", 4, 0, NULL };
        SF_CHUNK_ITERATOR *iter = sf_get_chunk_iterator(sndfile, &inf);

        /* If there's an issue getting the chunk or block alignment, load as
         * 16-bit and have libsndfile do the conversion.
         */
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
                /* Read the nBlockAlign field, and convert from bytes- to
                 * samples-per-block (verifying it's valid by converting back
                 * and comparing to the original value).
                 */
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

    /* Figure out the OpenAL format from the file and desired sample type. */
    format = AL_NONE;
    if (sfinfo.channels == 1)
    {
        if(sample_format == Int16)
            format = AL_FORMAT_MONO16;
        else if(sample_format == Float)
            format = AL_FORMAT_MONO_FLOAT32;
        else if(sample_format == IMA4)
            format = AL_FORMAT_MONO_IMA4;
        else if(sample_format == MSADPCM)
            format = AL_FORMAT_MONO_MSADPCM_SOFT;
    }
    else if (sfinfo.channels == 2)
    {
        if(sample_format == Int16)
            format = AL_FORMAT_STEREO16;
        else if(sample_format == Float)
            format = AL_FORMAT_STEREO_FLOAT32;
        else if(sample_format == IMA4)
            format = AL_FORMAT_STEREO_IMA4;
        else if(sample_format == MSADPCM)
            format = AL_FORMAT_STEREO_MSADPCM_SOFT;
    }
    else if (sfinfo.channels == 3)
    {
        if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
        {
            if(sample_format == Int16)
                format = AL_FORMAT_BFORMAT2D_16;
            else if(sample_format == Float)
                format = AL_FORMAT_BFORMAT2D_FLOAT32;
        }
    }
    else if(sfinfo.channels == 4)
    {
        if(sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
        {
            if(sample_format == Int16)
                format = AL_FORMAT_BFORMAT3D_16;
            else if(sample_format == Float)
                format = AL_FORMAT_BFORMAT3D_FLOAT32;
        }
    }
    if(!format)
    {
        fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
        sf_close(sndfile);
        return 0;
    }

    if(sfinfo.frames/splblockalign > (sf_count_t)(INT_MAX/byteblockalign))
    {
        fprintf(stderr, "Too many samples in %s (%" PRId64 ")\n", filename, sfinfo.frames);
        sf_close(sndfile);
        return 0;
    }

    /* Decode the whole audio file to a buffer. */
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
        if(num_frames > 0)
            num_frames = num_frames / byteblockalign * splblockalign;
    }
    
    if(num_frames < 1)
    {
        free(membuf);
        sf_close(sndfile);
        fprintf(stderr, "Failed to read samples in %s (%" PRId64 ")\n", filename, num_frames);
        return 0;
    }
    num_bytes = (ALsizei)(num_frames / splblockalign * byteblockalign);

    printf("Loading: %s (%s, %dhz)\n", filename, FormatName(format), sfinfo.samplerate);
    fflush(stdout);

    /* Buffer the audio data into a new buffer object, then free the data and
     * close the file.
     */
    buffer = 0;
    alGenBuffers(1, &buffer);
    if(splblockalign > 1)
        alBufferi(buffer, AL_UNPACK_BLOCK_ALIGNMENT_SOFT, splblockalign);
    alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

    free(membuf);
    sf_close(sndfile);

    /* Check if an error occured, and clean up if so. */
    err = alGetError();
    if(err != AL_NO_ERROR)
    {
        fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
        if(buffer && alIsBuffer(buffer))
            alDeleteBuffers(1, &buffer);
        return 0;
    }

    return buffer;
}

OpenALSound::OpenALSound(std::string filePath, float volume) : Sound(),
_buf(0),
_src(0)
{
    _buf = LoadSound(filePath.c_str());
    assert(_buf != AL_NONE);

    alGenSources(1, &_src);

    alSourcei(_src, AL_BUFFER, _buf);
    alSourcei(_src, AL_LOOPING, AL_FALSE);
}

OpenALSound::~OpenALSound()
{
    stop();

    alDeleteSources(1, &_src);
    alDeleteBuffers(1, &_buf);
}

void OpenALSound::play()
{
    alSourcei(_src, AL_LOOPING, _isLooping ? AL_TRUE : AL_FALSE);
    alSourcePlay(_src);
}

void OpenALSound::resume()
{
    if (isPaused())
    {
        alSourcePlay(_src);
    }
}

void OpenALSound::pause()
{
    if (isPlaying())
    {
        alSourcePause(_src);
    }
}

void OpenALSound::stop()
{
    alSourceStop(_src);
}

void OpenALSound::setVolume(float volume)
{
    alSourcef(_src, AL_GAIN, volume);
}

bool OpenALSound::isLooping()
{
    ALint isLooping;
    alGetSourcei(_src, AL_LOOPING, &isLooping);

    return isLooping;
}

bool OpenALSound::isPlaying()
{
    ALint state;
    alGetSourcei(_src, AL_SOURCE_STATE, &state);

    return state == AL_PLAYING;
}

bool OpenALSound::isPaused()
{
    return !isPlaying();
}

#endif /* IS_LINUX */

