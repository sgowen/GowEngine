//
//  ObjectALAudioEngine.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_APPLE

SoundWrapper* ObjectALAudioEngine::loadSound(std::string filePath, uint8_t numInstances)
{
    return new ObjectALSoundWrapper(filePath, false, numInstances);
}

SoundWrapper* ObjectALAudioEngine::loadMusic(std::string filePath)
{
    return new ObjectALSoundWrapper(filePath, true);
}

void ObjectALAudioEngine::onPause()
{
    pauseObjectAL();
}

void ObjectALAudioEngine::onResume()
{
    resumeObjectAL();
}

ObjectALAudioEngine::ObjectALAudioEngine() : GowAudioEngine()
{
    initObjectAL();
}

ObjectALAudioEngine::~ObjectALAudioEngine()
{
    deinitObjectAL();
}

#endif /* IS_APPLE */
