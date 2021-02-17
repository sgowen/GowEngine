//
//  EntityRenderController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/17/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "EntityRenderController.hpp"

#include "Entity.hpp"
#include "SpriteBatcher.hpp"
#include "Assets.hpp"

IMPL_RTTI_NOPARENT(EntityRenderController)
IMPL_EntityController_create(EntityRenderController, EntityRenderController)

EntityRenderController::EntityRenderController(Entity* e) :
_entity(e)
{
    // Empty
}

void EntityRenderController::addSprite(SpriteBatcher& sb)
{
    Entity& e = *_entity;
    TextureRegion tr = ASSETS.findTextureRegion(getTextureMapping(), e.stateTime());
    
    sb.addSprite(tr, e.getPosition()._x, e.getPosition()._y, e.width(), e.height(), e.getAngle(), e.isFacingLeft());
}

std::string EntityRenderController::getTextureMapping()
{
    Entity& e = *_entity;
    return getTextureMapping(e._state._state);
}

std::string EntityRenderController::getTextureMapping(uint8_t state)
{
    Entity& e = *_entity;
    auto q = e.entityDef()._textureMappings.find(state);
    if (q != e.entityDef()._textureMappings.end())
    {
        return q->second;
    }
    
    assert(false);
}

uint16_t EntityRenderController::getSoundMapping(uint8_t state)
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
        std::vector<uint16_t> soundCollection = q2->second;
        
        int index = rand() % soundCollection.size();
        return soundCollection[index];
    }
    
    // No sound for this state
    return 0;
}
