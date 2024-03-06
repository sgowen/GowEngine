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

LuaControllerAI::LuaControllerAI(Entity* e) : EntityControllerAI(e),
_lua(new sol::state())
{
    sol::state& lua = *_lua;
    
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os, sol::lib::package);
    
    lua.set_function("LOG", [](std::string line) {
        if (ENGINE_CFG.logLua())
        {
            LOG(line.c_str());
        }
    });
}

LuaControllerAI::~LuaControllerAI()
{
    delete _lua;
    _lua = nullptr;
}

void LuaControllerAI::runAI(uint16_t& inputState)
{
    Entity& e = *_entity;
    
    sol::state& lua = *_lua;
    
    lua.set_function("getNetworkUInt32", [&e](std::string name) {
        return e.networkDataField(name).valueUInt32();
    });
    
    lua.set_function("setInput", [&e, &inputState](std::string inputStateFlag, bool value) {
        uint16_t flag = e.inputStateFlag(inputStateFlag);
        SET_BIT(inputState, flag, value);
    });
    
    lua.set_function("findTargetWithinDistance", [&e](float shortestDistance) {
        Entity* target = nullptr;
        for (Entity* p : e.world()->getPlayers())
        {
            float distance = p->position().dist(e.position());
            if (distance < shortestDistance)
            {
                shortestDistance = distance;
                target = p;
            }
        }
        
        uint32_t ret = target ? target->getID() : 0;
        
        return ret;
    });
    
    lua.set_function("isPlayer", [&e](uint32_t entityID) {
        Entity* entity = e.world()->getEntityByID(entityID);
        
        bool ret = entity ? entity->isPlayer() : false;
        
        return ret;
    });
    
    lua.set_function("entityPositionX", [&e](uint32_t entityID) {
        Entity* entity = e.world()->getEntityByID(entityID);
        
        return entity ? entity->position()._x : 0;
    });
    
    lua.set_function("entityPositionY", [&e](uint32_t entityID) {
        Entity* entity = e.world()->getEntityByID(entityID);
        
        return entity ? entity->position()._y : 0;
    });
    
    lua.set_function("myPositionX", [&e]() {
        return e.position()._x;
    });
    
    lua.set_function("myPositionY", [&e]() {
        return e.position()._y;
    });
    
    Script& s = ASSETS_MGR.script(e.entityDef()._controllerAIScript);
    
    sol::load_result loadedScript = lua.load_buffer((const char*)s._fileData->_data, s._fileData->_length);
    sol::protected_function_result result = loadedScript();
    if (!result.valid() && ENGINE_CFG.logLua())
    {
        LOG("LuaControllerAI: %s is not valid", s._desc._name.c_str());
    }
}
