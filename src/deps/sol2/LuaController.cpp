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

LuaController::LuaController(Entity* e) : EntityController(e),
_lua(new sol::state())
{
    sol::state& lua = *_lua;
    
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os, sol::lib::package);
    
    lua.set_function("LOG", [](std::string line) {
        LOG(line.c_str());
    });
    
    lua.set_function("state", [&e]() {
        uint8_t stateVal = e->state()._state;
        LOG("stateVal: %d", stateVal);
        std::string stateStr = e->state(stateVal);
        LOG(stateStr);
        return stateStr;
    });
    
    Script& s = ASSETS_MGR.script(_entity->entityDef()._controllerScript);
    
    sol::load_result loadedScript = lua.load_buffer((const char*)s._fileData->_data, s._fileData->_length);
    sol::protected_function_result result = loadedScript();
    if (!result.valid() && ENGINE_CFG.logLua())
    {
        LOG("LuaController: %s is not valid", s._desc._name.c_str());
    }
    
    lua["onInit"]();
}

void LuaController::processInput(uint16_t inputState)
{
    sol::state& lua = *_lua;
    
    lua["processInput"](0);
}

void LuaController::onUpdate(uint32_t numMovesProcessed)
{
    sol::state& lua = *_lua;
    
    lua["onUpdate"](0);
}

void LuaController::onMessage(uint16_t message, Entity* fromEntity)
{
    if (fromEntity == nullptr)
    {
        return;
    }
    
    sol::state& lua = *_lua;
    
    lua["onMessage"]((int)message, fromEntity->getID());
}
