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
    
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os, sol::lib::package, sol::lib::string);
    
    lua.set_function("LOG", [](std::string line) {
        LOG(line.c_str());
    });
    
    Script& s = ASSETS_MGR.script(_entity->entityDef()._controllerScript);
    
    sol::load_result loadedScript = lua.load_buffer((const char*)s._fileData->_data, s._fileData->_length);
    sol::protected_function_result result = loadedScript();
    if (!result.valid() && ENGINE_CFG.logLua())
    {
        LOG("LuaController: %s is not valid", s._desc._name.c_str());
    }
    
    lua.set_function("setNetworkUInt8", [&e](std::string key, uint8_t value) {
        e->networkDataField("health").setValueUInt8(value);
    });
    
    lua["onInit"]();
}

void LuaController::processInput(uint16_t inputState)
{
    Entity& e = *_entity;
    sol::state& lua = *_lua;
    
    lua.set_function("stateStr", [&e]() {
        uint8_t stateVal = e.state()._state;
        std::string stateStr = e.state(stateVal);
        return stateStr;
    });
    
    lua.set_function("stateTime", [&e]() {
        uint16_t stateTimeVal = e.state()._stateTime;
        return stateTimeVal;
    });
    
    lua.set_function("isInputFlagSet", [&e, &inputState](std::string key) {
        uint16_t flag = e.inputStateFlag(key);
        bool ret = IS_BIT_SET(inputState, flag);
        
//        LOG("isInputFlagSet: %s %d %d -> %d", key.c_str(), inputState, flag, ret);
        
        return ret;
    });
    
    lua.set_function("wasInputFlagSet", [&e, &inputState](std::string key) {
        uint16_t flag = e.inputStateFlag(key);
        bool ret = IS_BIT_SET(e.lastProcessedInputState(), flag);
        
//        LOG("wasInputFlagSet: %s %d %d -> %d", key.c_str(), e.lastProcessedInputState(), flag, ret);
        
        return ret;
    });
    
    lua.set_function("getFloat", [&e](std::string key) {
        return e.data().getFloat(key);
    });
    
    lua.set_function("setState", [&e](std::string key) {
        uint8_t& state = e.state()._state;
        uint16_t& stateTime = e.state()._stateTime;
        
        state = e.state(key);
        stateTime = 0;
    });
    
    lua.set_function("isGrounded", [&e]() {
        return e.isGrounded();
    });
    
    lua.set_function("setVelocityX", [&e](float x) {
        Vector2& vel = e.velocity();
        vel._x = x;
    });
    
    lua.set_function("setVelocityY", [&e](float y) {
        Vector2& vel = e.velocity();
        vel._y = y;
    });
    
    lua.set_function("getVelocityY", [&e](float y) {
        Vector2& vel = e.velocity();
        return vel._y;
    });
    
    lua.set_function("setXFlipped", [&e](bool value) {
        e.pose()._isXFlipped = value;
    });
    
    lua.set_function("getNetworkUInt8", [&e, &inputState](std::string name) {
        return e.networkDataField(name).valueUInt8();
    });
    
    lua.set_function("getUInt", [&e](std::string key) {
        return e.data().getUInt(key);
    });
    
    lua["processInput"](inputState);
}

void LuaController::onUpdate(uint32_t numMovesProcessed)
{
    sol::state& lua = *_lua;
    
    Entity& e = *_entity;
    World& w = *e.world();
    Vector2& vel = e.velocity();
    uint8_t& state = e.state()._state;
    uint8_t& stateFlags = e.state()._stateFlags;
    uint16_t& stateTime = e.state()._stateTime;
    
    std::string textureMapping = e.controllerRender()->getTextureMapping();
    Animation* animation = ASSETS_MGR.animation(textureMapping);
    uint16_t animationNumFrames = animation == nullptr ? 1 : animation->cycleTime();
    
    lua.set_function("animationNumFrames", [&animationNumFrames]() {
        return animationNumFrames;
    });
    
    lua.set_function("exile", [&e]() {
        e.exile();
    });
    
    lua.set_function("getNetworkUInt32", [&e](std::string name) {
        return e.networkDataField(name).valueUInt32();
    });
    
    lua.set_function("sendMsgDamage", [&e, &w](uint32_t entityID) {
        Entity* entity = w.getEntityByID(entityID);
        if (entity != nullptr)
        {
            entity->message(MSG_DAMAGE, &e);
        }
    });
    
    lua.set_function("setStateFlag", [&e, &stateFlags](std::string stateFlag, bool value) {
        uint16_t flag = e.stateFlag(stateFlag);
        SET_BIT(stateFlags, flag, value);
    });
    
    lua["onUpdate"](numMovesProcessed);
}

void LuaController::onMessage(uint16_t message, Entity* fromEntity)
{
    sol::state& lua = *_lua;
    Entity& e = *_entity;
    World& w = *e.world();
    uint8_t& state = e.state()._state;
    uint16_t& stateTime = e.state()._stateTime;
    Vector2& vel = e.velocity();
    
    lua.set_function("isPlayer", [&w](uint32_t entityID) {
        Entity* entity = w.getEntityByID(entityID);
        
        bool ret = entity ? entity->isPlayer() : false;
        
        return ret;
    });
    
    lua.set_function("isMsgDangerousTouch", [&message]() {
        return message == MSG_DANGEROUS_TOUCH;
    });
    
    lua.set_function("isMsgNoTouch", [&message]() {
        return message == MSG_NO_TOUCH;
    });
    
    lua.set_function("isMsgDamage", [&message]() {
        return message == MSG_DAMAGE;
    });
    
    lua.set_function("setNetworkUInt32", [&e](std::string key, uint32_t value) {
        e.networkDataField(key).setValueUInt32(value);
    });
    
    lua.set_function("resetVelocity", [&vel]() {
        vel.reset();
    });
    
    lua.set_function("setNetworkUInt8", [&e](std::string key, uint8_t value) {
        e.networkDataField("health").setValueUInt8(value);
    });
    
    lua["onMessage"](message, fromEntity ? fromEntity->getID() : 0);
}
