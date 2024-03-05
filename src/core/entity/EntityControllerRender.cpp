//
//  EntityControllerRender.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/17/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI_NOPARENT(EntityControllerRender)
IMPL_EntityControllerRender_create_NOPARENT

EntityControllerRender::EntityControllerRender(Entity* e) :
_entity(e)
{
    // Empty
}

std::string EntityControllerRender::getTextureMapping()
{
    Entity& e = *_entity;
    return getTextureMapping(e.state()._state);
}

std::string EntityControllerRender::getTextureMapping(uint8_t state)
{
    Entity& e = *_entity;
    auto q = e.entityDef()._textureMappings.find(state);
    assert (q != e.entityDef()._textureMappings.end());
    
    return q->second;
}

std::vector<SoundMapping>* EntityControllerRender::getSoundMapping()
{
    Entity& e = *_entity;
    return getSoundMapping(e.state()._state);
}

std::vector<SoundMapping>* EntityControllerRender::getSoundMapping(uint8_t state)
{
    Entity& e = *_entity;
    auto q = e.entityDef()._soundMappings.find(state);
    if (q != e.entityDef()._soundMappings.end())
    {
        return &q->second;
    }
    
    return nullptr;
}
