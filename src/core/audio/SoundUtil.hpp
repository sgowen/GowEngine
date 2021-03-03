//
//  SoundUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Entity.hpp"
#include "EntityRenderController.hpp"
#include "AudioEngineFactory.hpp"

#include <stdint.h>

class SoundUtil
{
public:
    static void playSoundForStateIfChanged(Entity& e, uint8_t fromState, uint8_t toState)
    {
        if (fromState == toState)
        {
            return;
        }
        
        AUDIO_ENGINE.playSound(e.renderController()->getSoundMapping(toState));
    }
    
private:
    SoundUtil() {}
    ~SoundUtil() {}
    SoundUtil(const SoundUtil&);
    SoundUtil& operator=(const SoundUtil&);
};
