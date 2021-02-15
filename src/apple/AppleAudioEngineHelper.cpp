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

void AppleAudioEngineHelper::pause()
{
    pauseObjectAL();
}

void AppleAudioEngineHelper::resume()
{
    resumeObjectAL();
}

SoundWrapper* AppleAudioEngineHelper::loadSound(uint16_t soundID, const char *filePath, int numInstances)
{
	const char* bundlePath = getBundlePath(filePath);
    
    return new AppleSoundWrapper(soundID, bundlePath, numInstances);
}

SoundWrapper* AppleAudioEngineHelper::loadMusic(const char* filePath)
{
    return loadSound(1337, filePath);
}

AppleAudioEngineHelper::AppleAudioEngineHelper() : AudioEngineHelper()
{
    initObjectAL();
}

AppleAudioEngineHelper::~AppleAudioEngineHelper()
{
    deinitObjectAL();
}
