//
//  AppleAudioEngine.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "AppleAudioEngine.hpp"

#include "SoundWrapper.hpp"
#include "AppleSoundWrapper.hpp"
#include "ObjectALWrapper.hpp"
#include "NSAppleAssetHandler.hpp"

SoundWrapper* AppleAudioEngine::loadSound(const char *filePath, uint8_t numInstances)
{
	const char* bundlePath = getBundlePath(filePath);
    
    return new AppleSoundWrapper(bundlePath, false, numInstances);
}

SoundWrapper* AppleAudioEngine::loadMusic(const char* filePath)
{
    const char* bundlePath = getBundlePath(filePath);
    
    return new AppleSoundWrapper(bundlePath, true);
}

void AppleAudioEngine::pause()
{
    AudioEngine::pause();
    pauseObjectAL();
}

void AppleAudioEngine::resume()
{
    resumeObjectAL();
    AudioEngine::resume();
}

AppleAudioEngine::AppleAudioEngine() : AudioEngine()
{
    initObjectAL();
}

AppleAudioEngine::~AppleAudioEngine()
{
    deinitObjectAL();
}
