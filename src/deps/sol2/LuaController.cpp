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
    Vector2& vel = e.velocity();
    uint8_t& stateFlags = e.state()._stateFlags;
    Script& s = ASSETS_MGR.script(e.entityDef()._controllerScript);
    sol::state& lua = *s._lua;
    
    lua.set_function("isInputFlagSet", [&e, &inputState](std::string key) {
        uint16_t flag = e.inputStateFlag(key);
        bool ret = IS_BIT_SET(inputState, flag);
        
        return ret;
    });
    
    lua.set_function("wasInputFlagSet", [&e, &inputState](std::string key) {
        uint16_t flag = e.inputStateFlag(key);
        bool ret = IS_BIT_SET(e.lastProcessedInputState(), flag);
        
        return ret;
    });
    
    lua.set_function("setNetworkUInt8", [&e](std::string key, uint8_t value) {
        e.networkDataField("health").setValueUInt8(value);
    });
    
    lua.set_function("state", [&e]() {
        uint8_t stateVal = e.state()._state;
        return e.state(stateVal);
    });
    
    lua.set_function("stateTime", [&e]() {
        uint16_t stateTimeVal = e.state()._stateTime;
        return stateTimeVal;
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
    
    lua.set_function("getVelocityY", [&e]() {
        Vector2& vel = e.velocity();
        return vel._y;
    });
    
    lua.set_function("setXFlipped", [&e](bool value) {
        e.pose()._isXFlipped = value;
    });
    
    lua.set_function("isXFlipped", [&e]() {
        return e.pose()._isXFlipped;
    });
    
    lua.set_function("getNetworkUInt8", [&e](std::string name) {
        return e.networkDataField(name).valueUInt8();
    });
    
    lua.set_function("getUInt", [&e](std::string key) {
        return e.data().getUInt(key);
    });
    
    lua.set_function("exile", [&e]() {
        e.exile();
    });
    
    lua.set_function("getNetworkUInt32", [&e](std::string name) {
        return e.networkDataField(name).valueUInt32();
    });
    
    lua.set_function("getNetworkUInt16", [&e](std::string name) {
        return e.networkDataField(name).valueUInt16();
    });
    
    lua.set_function("sendMsgDamage", [&e](uint32_t entityID) {
        Entity* entity = e.world()->getEntityByID(entityID);
        if (entity != nullptr)
        {
            entity->message(MSG_DAMAGE, &e);
        }
    });
    
    lua.set_function("isPlayer", [&e](uint32_t entityID) {
        Entity* entity = e.world()->getEntityByID(entityID);
        
        bool ret = entity ? entity->isPlayer() : false;
        
        return ret;
    });
    
    lua.set_function("setNetworkUInt32", [&e](std::string key, uint32_t value) {
        e.networkDataField(key).setValueUInt32(value);
    });
    
    lua.set_function("setNetworkUInt16", [&e](std::string key, uint16_t value) {
        e.networkDataField(key).setValueUInt16(value);
    });
    
    lua.set_function("setZeroGravity", [&e](bool value) {
        e.pose()._isZeroGravity = value;
    });
    
    lua.set_function("setSlowed", [&e](bool value) {
        e.pose()._isSlowed = value;
    });
    
    lua.set_function("setNetworkUInt8", [&e](std::string key, uint8_t value) {
        e.networkDataField(key).setValueUInt8(value);
    });
    
    lua.set_function("setNetworkBool", [&e](std::string key, bool value) {
        e.networkDataField(key).setValueBool(value);
    });
    
    lua.set_function("getNetworkBool", [&e](std::string key) {
        return e.networkDataField(key).valueBool();
    });
    
    lua.set_function("setStateFlag", [&e, &stateFlags](std::string stateFlag, bool value) {
        uint16_t flag = e.stateFlag(stateFlag);
        SET_BIT(stateFlags, flag, value);
    });
    
    lua.set_function("isStateFlagSet", [&e, &stateFlags](std::string key) {
        bool ret = IS_BIT_SET(stateFlags, e.stateFlag(key));
        
        return ret;
    });
    
    lua.set_function("resetVelocity", [&vel]() {
        vel.reset();
    });
        
    lua["processInput"](inputState);
}

void LuaController::onUpdate(uint32_t numMovesProcessed)
{
    Entity& e = *_entity;
    Script& s = ASSETS_MGR.script(e.entityDef()._controllerScript);
    sol::state& lua = *s._lua;
    
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
    
    lua.set_function("isMsgDangerousTouch", [&message]() {
        return message == MSG_DANGEROUS_TOUCH;
    });
    
    lua.set_function("isMsgNoTouch", [&message]() {
        return message == MSG_NO_TOUCH;
    });
    
    lua.set_function("isMsgDamage", [&message]() {
        return message == MSG_DAMAGE;
    });
    
    lua["onMessage"](message, fromEntity ? fromEntity->getID() : 0);
}
