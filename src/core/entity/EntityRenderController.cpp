//
//  EntityRenderController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/17/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI_NOPARENT(EntityRenderController)
IMPL_EntityController_NOPARENT_create(EntityRenderController)

EntityRenderController::EntityRenderController(Entity* e) :
_entity(e)
{
    // Empty
}

std::string EntityRenderController::getTextureMapping()
{
    Entity& e = *_entity;
    return getTextureMapping(e._state._state, e._state._stateFlags);
}

std::string EntityRenderController::getTextureMapping(uint8_t state, uint8_t stateFlags)
{
    Entity& e = *_entity;
    auto q = e.entityDef()._textureMappings.find(state);
    assert (q != e.entityDef()._textureMappings.end());
    
    std::map<uint8_t, std::string>& stateTextureMappings = q->second;
    auto q2 = stateTextureMappings.find(stateFlags);
    assert (q2 != stateTextureMappings.end());
    
    return q2->second;
}

std::string EntityRenderController::getSoundMapping(uint8_t state)
{
    Entity& e = *_entity;
    auto q1 = e.entityDef()._soundMappings.find(state);
    if (q1 != e.entityDef()._soundMappings.end())
    {
        return q1->second;
    }
    
    auto q2 = e.entityDef()._soundRandomMappings.find(state);
    if (q2 != e.entityDef()._soundRandomMappings.end())
    {
        std::vector<std::string> soundCollection = q2->second;
        
        int index = rand() % soundCollection.size();
        return soundCollection[index];
    }
    
    return 0;
}
