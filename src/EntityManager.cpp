//
//  EntityManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/5/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "EntityManager.hpp"

#include "Entity.hpp"
#include "EntityLayoutManager.hpp"
#include "EntityController.hpp"
#include "EntityNetworkController.hpp"
#include "EntityPhysicsController.hpp"
#include "EntityRenderController.hpp"
#include "EntityController.hpp"
#include "StringUtil.hpp"
#include "STLUtil.hpp"
#include "AssetHandlerFactory.hpp"
#include "AssetHandler.hpp"
#include "FileData.hpp"
#include "RapidJSONUtil.hpp"

#include <rapidjson/document.h>

#include <assert.h>

void EntityManager::initWithJSONFile(const char* filePath)
{
    AssetHandler* ah = AssetHandlerFactory::create();
    FileData jsonData = ah->loadAsset(filePath);
    initWithJSON((const char*)jsonData._data);
    ah->releaseAsset(jsonData);
    AssetHandlerFactory::destroy(ah);
}

void EntityManager::initWithJSON(const char* json)
{
    _entityDescriptorsMap.clear();
    
    using namespace rapidjson;
    
    Document d;
    d.Parse<kParseStopWhenDoneFlag>(json);
    assert(d.IsObject());
    for (Value::ConstMemberIterator i = d.MemberBegin(); i != d.MemberEnd(); ++i)
    {
        const Value& iv = i->value;
        assert(iv.IsObject());
        
        std::string keyStr = i->name.GetString();
        uint32_t key = StringUtil::fourCharFromString(keyStr);
        
        assert(_entityDescriptorsMap.find(key) == _entityDescriptorsMap.end());
        
        std::string name = RapidJSONUtil::getString(iv, "name");
        std::string keyName = keyStr;
        std::string controller = RapidJSONUtil::getString(iv, "controller", "Default");
        std::string networkController = RapidJSONUtil::getString(iv, "networkController", "Default");
        std::string physicsController = RapidJSONUtil::getString(iv, "physicsController", "Default");
        std::string renderController = RapidJSONUtil::getString(iv, "renderController", "Default");
        
        std::map<uint8_t, std::string> textureMappings;
        if (iv.HasMember("textureMappings"))
        {
            const Value& v = iv["textureMappings"];
            assert(v.IsObject());
            for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
            {
                std::string name = i->name.GetString();
                uint32_t nameVal = StringUtil::stringToNumber<uint32_t>(name);
                uint8_t state = nameVal;
                std::string value = i->value.GetString();
                textureMappings.emplace(state, value);
            }
        }
        else if (iv.HasMember("textureMapping"))
        {
            const Value& v = iv["textureMapping"];
            assert(v.IsString());
            uint8_t state = 0;
            std::string value = v.GetString();
            textureMappings.emplace(state, value);
        }
        
        std::map<uint8_t, uint16_t> soundMappings;
        if (iv.HasMember("soundMappings"))
        {
            const Value& v = iv["soundMappings"];
            assert(v.IsObject());
            for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
            {
                std::string name = i->name.GetString();
                uint32_t nameVal = StringUtil::stringToNumber<uint32_t>(name);
                uint8_t state = nameVal;
                assert(i->value.IsUint());
                uint16_t soundID = i->value.GetUint();
                soundMappings.emplace(state, soundID);
            }
        }
        
        std::map<uint8_t, std::vector<uint16_t> > soundRandomMappings;
        if (iv.HasMember("soundRandomMappings"))
        {
            const Value& v = iv["soundRandomMappings"];
            assert(v.IsObject());
            for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
            {
                assert(i->value.IsArray());
                const Value& array = i->value.GetArray();
                std::vector<uint16_t> soundCollection;
                for (SizeType i = 0; i < array.Size(); ++i)
                {
                    const Value& iv = array[i];
                    assert(iv.IsUint());
                    soundCollection.push_back(iv.GetUint());
                }
                std::string name = i->name.GetString();
                uint32_t nameVal = StringUtil::stringToNumber<uint32_t>(name);
                uint8_t state = nameVal;
                soundRandomMappings.emplace(state, soundCollection);
            }
        }
        
        std::vector<FixtureDef> fixtures;
        if (iv.HasMember("fixtures"))
        {
            const Value& v = iv["fixtures"];
            assert(v.IsArray());
            for (SizeType i = 0; i < v.Size(); ++i)
            {
                const Value& iv = v[i];
                assert(iv.IsObject());
                FixtureDef fixtureDef;
                fixtureDef._restitution = RapidJSONUtil::getFloat(iv, "restitution");
                fixtureDef._density = RapidJSONUtil::getFloat(iv, "density");
                fixtureDef._friction = RapidJSONUtil::getFloat(iv, "friction");
                fixtureDef._flags = RapidJSONUtil::getInt(iv, "flags");
                
                if (iv.HasMember("vertices"))
                {
                    const Value& iiv = iv["vertices"];
                    assert(iiv.IsArray());
                    for (SizeType i = 0; i < iiv.Size(); ++i)
                    {
                        const Value& iiiv = iiv[i];
                        assert(iiiv.IsObject());
                        float x = RapidJSONUtil::getFloat(iiiv, "x");
                        float y = RapidJSONUtil::getFloat(iiiv, "y");
                        fixtureDef._vertices.emplace_back(x, y);
                    }
                }
                
                float x = 0;
                float y = 0;
                if (iv.HasMember("center"))
                {
                    const Value& iiv = iv["center"];
                    assert(iiv.IsObject());
                    x = RapidJSONUtil::getFloat(iiv, "x");
                    y = RapidJSONUtil::getFloat(iiv, "y");
                }
                fixtureDef._center.set(x, y);
                
                fixtures.push_back(fixtureDef);
            }
        }
        uint8_t bodyFlags = RapidJSONUtil::getUInt(iv, "bodyFlags");
        uint8_t width = RapidJSONUtil::getUInt(iv, "width");
        uint8_t height = RapidJSONUtil::getUInt(iv, "height");
        
        Config cfg;
        if (iv.HasMember("data"))
        {
            std::map<std::string, std::string> keyValues;
            const Value& v = iv["data"];
            assert(v.IsObject());
            for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
            {
                assert(i->value.IsString());
                
                keyValues[i->name.GetString()] = i->value.GetString();
            }
            cfg.initWithKeyValues(keyValues);
        }
        
        _entityDescriptorsMap.emplace(key, EntityDef{key, name, keyName, controller, networkController, physicsController, renderController, textureMappings, soundMappings, soundRandomMappings, fixtures, bodyFlags, width, height, cfg});
    }
}

Entity* EntityManager::createEntity(EntityInstanceDef eid)
{
    EntityDef& ed = getEntityDef(eid._key);
    return new Entity(ed, eid);
}

EntityDef& EntityManager::getEntityDef(uint32_t fourCCName)
{
    auto q = _entityDescriptorsMap.find(fourCCName);
    assert(q != _entityDescriptorsMap.end());
    return q->second;
}

void EntityManager::registerController(std::string name, EntityControllerCreationFunc func)
{
    assert(func != NULL);
    _entityControllerCreationFunctionMap[name] = func;
}

EntityController* EntityManager::createEntityController(EntityDef& ed, Entity* e)
{
    EntityControllerCreationFunc func = _entityControllerCreationFunctionMap[ed._controller];
    assert(func != NULL);
    return func(e);
}

const std::map<std::string, EntityControllerCreationFunc>& EntityManager::getEntityControllerMap()
{
    return _entityControllerCreationFunctionMap;
}

void EntityManager::registerNetworkController(std::string name, EntityNetworkControllerCreationFunc func)
{
    assert(func != NULL);
    _entityNetworkControllerCreationFunctionMap[name] = func;
}

EntityNetworkController* EntityManager::createEntityNetworkController(EntityDef& ed, Entity* e)
{
    EntityNetworkControllerCreationFunc func = _entityNetworkControllerCreationFunctionMap[ed._networkController];
    assert(func != NULL);
    return func(e);
}

const std::map<std::string, EntityNetworkControllerCreationFunc>& EntityManager::getEntityNetworkControllerMap()
{
    return _entityNetworkControllerCreationFunctionMap;
}

void EntityManager::registerPhysicsController(std::string name, EntityPhysicsControllerCreationFunc func)
{
    assert(func != NULL);
    _entityPhysicsControllerCreationFunctionMap[name] = func;
}

EntityPhysicsController* EntityManager::createEntityPhysicsController(EntityDef& ed, Entity* e)
{
    EntityPhysicsControllerCreationFunc func = _entityPhysicsControllerCreationFunctionMap[ed._physicsController];
    assert(func != NULL);
    return func(e);
}

const std::map<std::string, EntityPhysicsControllerCreationFunc>& EntityManager::getEntityPhysicsControllerMap()
{
    return _entityPhysicsControllerCreationFunctionMap;
}

void EntityManager::registerRenderController(std::string name, EntityRenderControllerCreationFunc func)
{
    assert(func != NULL);
    _entityRenderControllerCreationFunctionMap[name] = func;
}

EntityRenderController* EntityManager::createEntityRenderController(EntityDef& ed, Entity* e)
{
    EntityRenderControllerCreationFunc func = _entityRenderControllerCreationFunctionMap[ed._renderController];
    assert(func != NULL);
    return func(e);
}

const std::map<std::string, EntityRenderControllerCreationFunc>& EntityManager::getEntityRenderControllerMap()
{
    return _entityRenderControllerCreationFunctionMap;
}

EntityManager::EntityManager()
{
    registerController("Default", EntityController::create);
    registerNetworkController("Default", EntityNetworkController::create);
    registerPhysicsController("Default", EntityPhysicsController::create);
    registerRenderController("Default", EntityRenderController::create);
}
