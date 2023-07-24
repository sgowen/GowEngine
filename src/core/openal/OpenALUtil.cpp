//
//  OpenALUtil.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/11/23.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

void OpenALUtil::openDeviceAndCreateContext()
{
    bool isSuccessful = true;
    
    const ALCchar *name;
    ALCdevice *device;
    ALCcontext *ctx;

    device = alcOpenDevice(nullptr);
    if (!device)
    {
        LOG("Could not open a device!");
        isSuccessful = false;
    }

    if (isSuccessful)
    {
        ctx = alcCreateContext(device, nullptr);
        if (ctx == nullptr || alcMakeContextCurrent(ctx) == ALC_FALSE)
        {
            if (ctx != nullptr)
            {
                alcDestroyContext(ctx);
            }
            alcCloseDevice(device);
            LOG("Could not set a context!");
            isSuccessful = false;
        }
    }
    
    if (isSuccessful)
    {
        name = nullptr;
        if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
        {
            name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
        }
        
        if (!name || alcGetError(device) != AL_NO_ERROR)
        {
            name = alcGetString(device, ALC_DEVICE_SPECIFIER);
        }
        
        LOG("Opened \"%s\"", name);
    }
    
    assert(isSuccessful);
}

void OpenALUtil::destroyContextAndCloseDevice()
{
    bool isSuccessful = false;
    
    ALCdevice *device;
    ALCcontext *ctx;

    ctx = alcGetCurrentContext();
    if (ctx != nullptr)
    {
        device = alcGetContextsDevice(ctx);

        alcMakeContextCurrent(nullptr);
        alcDestroyContext(ctx);
        alcCloseDevice(device);
        
        isSuccessful = true;
    }
    
    assert(isSuccessful);
}

const char* formatName(ALenum format)
{
    switch (format)
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

ALenum convertToOpenALFormat(Format format)
{
    switch (format)
    {
        case Format_MONO16:
            return AL_FORMAT_MONO16;
        case Format_MONO_FLOAT32:
            return AL_FORMAT_MONO_FLOAT32;
        case Format_MONO_IMA4:
            return AL_FORMAT_MONO_IMA4;
        case Format_MONO_MSADPCM_SOFT:
            return AL_FORMAT_MONO_MSADPCM_SOFT;
        case Format_STEREO16:
            return AL_FORMAT_STEREO16;
        case Format_STEREO_FLOAT32:
            return AL_FORMAT_STEREO_FLOAT32;
        case Format_STEREO_IMA4:
            return AL_FORMAT_STEREO_IMA4;
        case Format_STEREO_MSADPCM_SOFT:
            return AL_FORMAT_STEREO_MSADPCM_SOFT;
        case Format_BFORMAT2D_16:
            return AL_FORMAT_BFORMAT2D_16;
        case Format_BFORMAT2D_FLOAT32:
            return AL_FORMAT_BFORMAT2D_FLOAT32;
        case Format_BFORMAT3D_16:
            return AL_FORMAT_BFORMAT3D_16;
        case Format_BFORMAT3D_FLOAT32:
            return AL_FORMAT_BFORMAT3D_FLOAT32;
        case Format_NONE:
        default:
            return AL_NONE;
    }
}

void crashIfError()
{
    ALenum err = AL_NO_ERROR;
    err = alGetError();
    if (err != AL_NO_ERROR)
    {
        LOG("OpenAL Error: %s", alGetString(err));
    }
    assert(err == AL_NO_ERROR);
}

void OpenALUtil::loadSound(Sound& s)
{
    ALenum format = convertToOpenALFormat(s._format);
    if (ENGINE_CFG.fileLoggingEnabled())
    {
        LOG("OpenALUtil::loadSound: %s (%s, %dhz)", s._desc._filePath.c_str(), formatName(format), s._sampleRate);
    }
    
    if (s._formatType == FormatType_Float)
    {
        assert(alIsExtensionPresent("AL_EXT_FLOAT32"));
    }
    else if (s._formatType == FormatType_IMA4)
    {
        assert(alIsExtensionPresent("AL_EXT_IMA4")
               && alIsExtensionPresent("AL_SOFT_block_alignment"));
    }
    else if (s._formatType == FormatType_MSADPCM)
    {
        assert(alIsExtensionPresent("AL_SOFT_MSADPCM")
               && alIsExtensionPresent("AL_SOFT_block_alignment"));
    }
    
    ALuint buffer = 0;
    alGenBuffers(1, &buffer);
    assert(buffer != AL_NONE);
    crashIfError();
    
    if (s._splblockalign > 1)
    {
        alBufferi(buffer, AL_UNPACK_BLOCK_ALIGNMENT_SOFT, s._splblockalign);
        crashIfError();
    }
    
    alBufferData(buffer, format, s._data, s._numBytes, s._sampleRate);
    crashIfError();
    
    ALsizei numInstances = s._desc._numInstances;
    s._alHandles = (uint32_t*) malloc(numInstances * sizeof(uint32_t));
    alGenSources(numInstances, s._alHandles);
    crashIfError();

    for (int i = 0; i < numInstances; ++i)
    {
        alSourcei(s._alHandles[i], AL_BUFFER, buffer);
        crashIfError();
    }
}

void OpenALUtil::unloadSound(Sound& s)
{
    if (ENGINE_CFG.fileLoggingEnabled())
    {
        LOG("OpenALUtil::unloadSound %s", s._desc._filePath.c_str());
    }
    
    ALsizei numInstances = s._desc._numInstances;
    for (int i = 0; i < numInstances; ++i)
    {
        stop(s._alHandles[i]);
    }

    ALint bufferFromSource = 0;
    alGetSourcei(s._alHandles[0], AL_BUFFER, &bufferFromSource);
    crashIfError();
    
    alDeleteSources(numInstances, s._alHandles);
    crashIfError();
    
    free(s._alHandles);
    s._alHandles = nullptr;
    
    ALuint buffer = bufferFromSource;
    alDeleteBuffers(1, &buffer);
    crashIfError();
}

void OpenALUtil::play(uint32_t alHandle)
{
    alSourcePlay(alHandle);
    crashIfError();
}

void OpenALUtil::resume(uint32_t alHandle)
{
    if (isPaused(alHandle))
    {
        alSourcePlay(alHandle);
        crashIfError();
    }
}

void OpenALUtil::pause(uint32_t alHandle)
{
    if (isPlaying(alHandle))
    {
        alSourcePause(alHandle);
        crashIfError();
    }
}

void OpenALUtil::stop(uint32_t alHandle)
{
    alSourceStop(alHandle);
    crashIfError();
}

void OpenALUtil::setVolume(uint32_t alHandle, float volume)
{
    alSourcef(alHandle, AL_GAIN, volume);
    crashIfError();
}

bool OpenALUtil::isLooping(uint32_t alHandle)
{
    ALint isLooping;
    alGetSourcei(alHandle, AL_LOOPING, &isLooping);
    crashIfError();

    return isLooping == 1;
}

void OpenALUtil::setLooping(uint32_t alHandle, bool isLooping)
{
    alSourcei(alHandle, AL_LOOPING, isLooping ? AL_TRUE : AL_FALSE);
    crashIfError();
}

void OpenALUtil::seekAhead(uint32_t alHandle, uint32_t numFrames)
{
    static float frameRate = static_cast<float>(ENGINE_CFG.frameRate());
    alSourcef(alHandle, AL_SEC_OFFSET, numFrames * frameRate);
    crashIfError();
}

bool OpenALUtil::isPlaying(uint32_t alHandle)
{
    ALint state;
    alGetSourcei(alHandle, AL_SOURCE_STATE, &state);
    crashIfError();

    return state == AL_PLAYING;
}

bool OpenALUtil::isPaused(uint32_t alHandle)
{
    ALint state;
    alGetSourcei(alHandle, AL_SOURCE_STATE, &state);
    crashIfError();

    return state == AL_PAUSED;
}
