//
//  LuaControllerAI.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/04/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#define SOL_USING_CXX_LUA 1
#include <sol/sol.hpp>

IMPL_RTTI(LuaControllerAI, EntityControllerAI)
IMPL_EntityControllerAI_create(LuaControllerAI)

LuaControllerAI::LuaControllerAI(Entity* e) : EntityControllerAI(e)
{
    // Empty
}

void LuaControllerAI::runAI(uint16_t& inputState)
{
    Entity& e = *_entity;
    Script& s = ASSETS_MGR.script(e.entityDef()._controllerAIScript);
    sol::state& lua = *s._lua;
    
    LUA.bindToEntity(lua, e);
    
    lua.set_function("setInput", [&e, &inputState](std::string inputStateFlag, bool value) {
        uint16_t flag = e.inputStateFlag(inputStateFlag);
        SET_BIT(inputState, flag, value);
    });
    
    lua["runAI"]();
}
