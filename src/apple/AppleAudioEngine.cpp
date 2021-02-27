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
#include "ObjectALHelper.hpp"
#include "NSBundleHelper.hpp"

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

void AppleAudioEngine::onPause()
{
    pauseObjectAL();
}

void AppleAudioEngine::onResume()
{
    resumeObjectAL();
}

AppleAudioEngine::AppleAudioEngine() : AudioEngine()
{
    initObjectAL();
}

AppleAudioEngine::~AppleAudioEngine()
{
    deinitObjectAL();
}
