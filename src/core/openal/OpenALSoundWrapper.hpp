//
//  OpenALSoundWrapper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/audio/SoundWrapper.hpp"
#include "core/common/PlatformMacros.hpp"

#if IS_LINUX

class OpenALSoundWrapper : public SoundWrapper
{
public:
    OpenALSoundWrapper(std::string filePath, uint8_t numInstances = 1);
    virtual ~OpenALSoundWrapper() {}
};

#endif /* IS_LINUX */
