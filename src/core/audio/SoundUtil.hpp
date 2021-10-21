//
//  SoundUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/entity/Entity.hpp"
#include "core/entity/EntityRenderController.hpp"
#include "core/network/NetworkClient.hpp"
#include "core/common/StringUtil.hpp"

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
        
        playSound(e.position(), e.world(), e.renderController()->getSoundMapping(toState));
    }
    
    static void playSound(Vector2& position, World* world, std::string soundID)
    {
        if (soundID.empty())
        {
            return;
        }
        
        float volume = 1;
        float playerVolume = 1;
        
        std::vector<Entity*>& players = world->getPlayers();
        for (Entity* e : players)
        {
            uint8_t playerID = e->dataField("playerID").valueUInt8();
            if (!NW_CLNT->isPlayerIDLocal(playerID))
            {
                // Distance attenuated sound is
                // measured from your local player(s)
                continue;
            }
            
            if (e->position() == position)
            {
                continue;
            }
            
            float distance = e->position().dist(position);
            if (distance > 0)
            {
                float factor = distance / (e->width() * 4);
                float newPlayerVolume = 1.0f / (factor * factor);
                if (newPlayerVolume < playerVolume)
                {
                    playerVolume = newPlayerVolume;
                }
            }
        }
        
        volume = playerVolume;
        if (volume < 0.1f)
        {
            volume = 0.0f;
        }
        
        AUDIO_ENGINE.playSound(soundID, volume);
    }
    
private:
    SoundUtil() {}
    ~SoundUtil() {}
    SoundUtil(const SoundUtil&);
    SoundUtil& operator=(const SoundUtil&);
};
