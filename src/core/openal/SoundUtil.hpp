//
//  SoundUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>
#include <string>

class Entity;
class World;
class Vector2;

class SoundUtil
{
public:
    static void playSoundForStateIfChanged(Entity& e, uint8_t fromState, uint8_t toState);
    static void playSound(Vector2& position, World* world, std::string soundID);
    
private:
    SoundUtil() {}
    ~SoundUtil() {}
    SoundUtil(const SoundUtil&);
    SoundUtil& operator=(const SoundUtil&);
};
