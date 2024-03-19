//
//  LuaController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/24/24.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#define SOL_USING_CXX_LUA 1
#include <sol/sol.hpp>

IMPL_RTTI(LuaController, EntityController)
IMPL_EntityController_create(LuaController)

LuaController::LuaController(Entity* entity) : EntityController(entity)
{
    // Empty
}

void LuaController::processInput(uint16_t inputState)
{
    Entity& e = *_entity;
    Script& s = ASSETS_MGR.script(e.entityDef()._controllerScript);
    sol::state& lua = *s._lua;
    
    LUA.bindToEntity(lua, e);
    
    lua.set_function("isInputFlagSet", [&e, &inputState](std::string key) {
        uint16_t flag = e.inputStateFlag(key);
        bool ret = IS_BIT_SET(inputState, flag);
        
        return ret;
    });
        
    lua["processInput"](inputState);
}

void LuaController::onUpdate(uint32_t numMovesProcessed)
{
    Entity& e = *_entity;
    Script& s = ASSETS_MGR.script(e.entityDef()._controllerScript);
    sol::state& lua = *s._lua;
    
    LUA.bindToEntity(lua, e);
    
    std::string textureMapping = e.controllerRender()->getTextureMapping();
    Animation* animation = ASSETS_MGR.animation(textureMapping);
    uint16_t animationNumFrames = animation == nullptr ? 1 : animation->cycleTime();
    uint16_t animationNumFramesBeforeFirstLoopingFrame = animation == nullptr ? 1 : animation->cycleTimeBeforeFirstLoopingFrame();
    
    lua.set_function("animationNumFrames", [&animationNumFrames]() {
        return animationNumFrames;
    });
    
    lua.set_function("animationNumFramesBeforeFirstLoopingFrame", [&animationNumFramesBeforeFirstLoopingFrame]() {
        return animationNumFramesBeforeFirstLoopingFrame;
    });
    
    lua["onUpdate"](numMovesProcessed);
}

void LuaController::onMessage(uint16_t message, Entity* fromEntity)
{
    Entity& e = *_entity;
    Script& s = ASSETS_MGR.script(e.entityDef()._controllerScript);
    sol::state& lua = *s._lua;
    
    LUA.bindToEntity(lua, e);
    
    lua["onMessage"](e.message(message), fromEntity ? fromEntity->getID() : 0);
}
