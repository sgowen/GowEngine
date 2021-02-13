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
#include "EntityPhysicsController.hpp"
#include "EntityNetworkController.hpp"

#include "EntityController.hpp"
#include "StringUtil.hpp"
#include "GowUtil.hpp"
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
    clear();
    
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
        std::string controller = RapidJSONUtil::getString(iv, "controller", "Entity");
        std::string networkController = RapidJSONUtil::getString(iv, "networkController", "Entity");
        std::string physicsController = RapidJSONUtil::getString(iv, "physicsController", "Entity");
        
        std::map<int, std::string> textureMappings;
        if (iv.HasMember("textureMappings"))
        {
            const Value& v = iv["textureMappings"];
            assert(v.IsObject());
            for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
            {
                std::string name = i->name.GetString();
                int state = StringUtil::stringToNumber<int>(name);
                std::string value = i->value.GetString();
                textureMappings.insert(std::make_pair(state, value));
            }
        }
        else
        {
            const Value& v = iv["textureMapping"];
            assert(v.IsString());
            std::string value = v.GetString();
            textureMappings.insert(std::make_pair(0, value));
        }
        
        std::map<int, int> soundMappings;
        if (iv.HasMember("soundMappings"))
        {
            const Value& v = iv["soundMappings"];
            assert(v.IsObject());
            for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
            {
                std::string name = i->name.GetString();
                int state = StringUtil::stringToNumber<int>(name);
                int soundID = i->value.GetInt();
                soundMappings.insert(std::make_pair(state, soundID));
            }
        }
        
        std::map<int, std::vector<int> > soundRandomMappings;
        if (iv.HasMember("soundRandomMappings"))
        {
            const Value& v = iv["soundRandomMappings"];
            assert(v.IsObject());
            for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
            {
                assert(i->value.IsArray());
                const Value& array = i->value.GetArray();
                std::vector<int> soundCollection;
                for (SizeType i = 0; i < array.Size(); ++i)
                {
                    const Value& iv = array[i];
                    assert(iv.IsInt());
                    soundCollection.push_back(iv.GetInt());
                }
                std::string name = i->name.GetString();
                int state = StringUtil::stringToNumber<int>(name);
                soundRandomMappings.insert(std::make_pair(state, soundCollection));
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
                fixtureDef._flags = RapidJSONUtil::getInteger(iv, "flags");
                
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
        int bodyFlags = RapidJSONUtil::getInteger(iv, "bodyFlags");
        int width = RapidJSONUtil::getInteger(iv, "width");
        int height = RapidJSONUtil::getInteger(iv, "height");
        
        _entityDescriptorsMap.emplace(key, EntityDef{key, name, keyName, controller, physicsController, networkController, textureMappings, soundMappings, soundRandomMappings, fixtures, bodyFlags, width, height});
    }
}

void EntityManager::clear()
{
    _entityDescriptorsMap.clear();
}

Entity* EntityManager::createEntity(EntityInstanceDef& eid, bool isServer)
{
    EntityDef& ed = getEntityDef(eid._key);
    return new Entity(ed, eid, isServer);
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

void EntityManager::registerNetworkController(std::string name, EntityNetworkControllerCreationFunc func)
{
    assert(func != NULL);
    
    _entityNetworkControllerCreationFunctionMap[name] = func;
}

void EntityManager::registerPhysicsController(std::string name, EntityPhysicsControllerCreationFunc func)
{
    assert(func != NULL);
    
    _entityPhysicsControllerCreationFunctionMap[name] = func;
}

EntityController* EntityManager::createEntityController(EntityDef& ed, Entity* e)
{
    EntityControllerCreationFunc func = _entityControllerCreationFunctionMap[ed._controller];
    assert(func != NULL);
    
    return func(e);
}

EntityNetworkController* EntityManager::createEntityNetworkController(EntityDef& ed, Entity* e, bool isServer)
{
    EntityNetworkControllerCreationFunc func = _entityNetworkControllerCreationFunctionMap[ed._networkController];
    assert(func != NULL);
    
    return func(e, isServer);
}

EntityPhysicsController* EntityManager::createEntityPhysicsController(EntityDef& ed, Entity* e)
{
    EntityPhysicsControllerCreationFunc func = _entityPhysicsControllerCreationFunctionMap[ed._physicsController];
    assert(func != NULL);
    
    return func(e);
}

const std::map<std::string, EntityControllerCreationFunc>& EntityManager::getEntityControllerMap()
{
    return _entityControllerCreationFunctionMap;
}

const std::map<std::string, EntityNetworkControllerCreationFunc>& EntityManager::getEntityNetworkControllerMap()
{
    return _entityNetworkControllerCreationFunctionMap;
}

const std::map<std::string, EntityPhysicsControllerCreationFunc>& EntityManager::getEntityPhysicsControllerMap()
{
    return _entityPhysicsControllerCreationFunctionMap;
}

EntityManager::EntityManager()
{
    registerController("Entity", EntityController::create);
    registerNetworkController("Entity", EntityNetworkController::create);
    registerPhysicsController("Entity", EntityPhysicsController::create);
}
