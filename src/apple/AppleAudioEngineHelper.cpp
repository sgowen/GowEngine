//
//  AppleAudioEngineHelper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "AppleAudioEngineHelper.hpp"

#include "SoundWrapper.hpp"
#include "AppleSoundWrapper.hpp"
#include "ObjectALWrapper.hpp"
#include "NSAppleAssetHandler.hpp"

SoundWrapper* AppleAudioEngineHelper::loadSound(const char *filePath, uint8_t numInstances)
{
	const char* bundlePath = getBundlePath(filePath);
    
    return new AppleSoundWrapper(bundlePath, false, numInstances);
}

SoundWrapper* AppleAudioEngineHelper::loadMusic(const char* filePath)
{
    const char* bundlePath = getBundlePath(filePath);
    
    return new AppleSoundWrapper(bundlePath, true);
}

void AppleAudioEngineHelper::pause()
{
    pauseObjectAL();
}

void AppleAudioEngineHelper::resume()
{
    resumeObjectAL();
}

AppleAudioEngineHelper::AppleAudioEngineHelper() : AudioEngineHelper()
{
    initObjectAL();
}

AppleAudioEngineHelper::~AppleAudioEngineHelper()
{
    deinitObjectAL();
}
