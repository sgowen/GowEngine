//
//  ObjectALSoundWrapper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildMacros.hpp>
#if IS_APPLE

#include "core/audio/SoundWrapper.hpp"

class ObjectALSoundWrapper : public SoundWrapper
{
public:
    ObjectALSoundWrapper(std::string filePath, bool isMusic, uint8_t numInstances = 1);
    virtual ~ObjectALSoundWrapper() {}
};

#endif /* IS_APPLE */
