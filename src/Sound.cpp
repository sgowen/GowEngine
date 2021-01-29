//
//  Sound.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/10/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "Sound.hpp"

Sound::Sound(uint16_t soundID) :
_soundID(soundID)
{
    // Empty
}

uint16_t Sound::getSoundID()
{
    return _soundID;
}
