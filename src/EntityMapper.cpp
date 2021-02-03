//
//  EntityMapper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/5/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "EntityMapper.hpp"

#include "Entity.hpp"
#include "EntityLayoutMapper.hpp"
#include "EntityController.hpp"
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

void EntityMapper::initWithJSONFile(const char* filePath)
{
    AssetHandler* ah = AssetHandlerFactory::create();
    FileData jsonData = ah->loadAsset(filePath);
    initWithJSON((const char*)jsonData._data);
    ah->releaseAsset(jsonData);
    AssetHandlerFactory::destroy(ah);
}

void EntityMapper::initWithJSON(const char* json)
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
        
        EntityDef* entry = new EntityDef();
        
        entry->_key = key;
        entry->_name = RapidJSONUtil::getString(iv, "name");
        entry->_keyName = keyStr;
        entry->_controller = RapidJSONUtil::getString(iv, "entityController", "Entity");
        entry->_networkController = RapidJSONUtil::getString(iv, "entityNetworkController", "Entity");
        
        if (iv.HasMember("textureMappings"))
        {
            const Value& v = iv["textureMappings"];
            assert(v.IsObject());
            for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
            {
                std::string name = i->name.GetString();
                int state = StringUtil::stringToNumber<int>(name);
                std::string value = i->value.GetString();
                entry->_textureMappings.insert(std::make_pair(state, value));
            }
        }
        else
        {
            const Value& v = iv["textureMapping"];
            assert(v.IsString());
            std::string value = v.GetString();
            entry->_textureMappings.insert(std::make_pair(0, value));
        }
        
        if (iv.HasMember("soundMappings"))
        {
            const Value& v = iv["soundMappings"];
            assert(v.IsObject());
            for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
            {
                std::string name = i->name.GetString();
                int state = StringUtil::stringToNumber<int>(name);
                int soundID = i->value.GetInt();
                entry->_soundMappings.insert(std::make_pair(state, soundID));
            }
        }
        
        if (iv.HasMember("soundCollectionMappings"))
        {
            const Value& v = iv["soundCollectionMappings"];
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
                entry->_soundCollectionMappings.insert(std::make_pair(state, soundCollection));
            }
        }
        
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
                        fixtureDef._vertices.push_back(b2Vec2(x, y));
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
                fixtureDef._center.Set(x, y);
                
                entry->_fixtures.push_back(fixtureDef);
            }
        }
        entry->_bodyFlags = RapidJSONUtil::getInteger(iv, "bodyFlags");
        entry->_width = RapidJSONUtil::getInteger(iv, "width");
        entry->_height = RapidJSONUtil::getInteger(iv, "height");
        entry->_stateSensitive = RapidJSONUtil::getBool(iv, "stateSensitive");
        
        _entityDescriptorsMap[key] = entry;
        _entityDescriptors.push_back(entry);
    }
}

void EntityMapper::clear()
{
    GowUtil::cleanUpMapOfPointers(_entityDescriptorsMap);
    _entityDescriptors.clear();
}

Entity* EntityMapper::createEntity(EntityInstanceDef* eid, bool isServer)
{
    return createEntityFromDef(getEntityDef(eid->_key), eid, isServer);
}

Entity* EntityMapper::createEntityFromDef(EntityDef* entityDef, EntityInstanceDef* eid, bool isServer)
{
    EntityDef ed = *entityDef;
    
    ed._entityID = eid->_entityID;
    ed._x = eid->_x;
    ed._y = eid->_y;
    ed._width = eid->_w > 0 ? eid->_w : entityDef->_width;
    ed._height = eid->_h > 0 ? eid->_h : entityDef->_height;
    ed._server = isServer;
    
    return new Entity(ed);
}

EntityDef* EntityMapper::getEntityDef(uint32_t fourCCName)
{
    auto q = _entityDescriptorsMap.find(fourCCName);
    
    assert(q != _entityDescriptorsMap.end());
    
    EntityDef* ret = q->second;
    
    return ret;
}

void EntityMapper::registerEntityController(std::string name, EntityControllerCreationFunc func)
{
    assert(func);
    
    _entityControllerCreationFunctionMap[name] = func;
}

void EntityMapper::registerEntityNetworkController(std::string name, EntityNetworkControllerCreationFunc func)
{
    assert(func);
    
    _entityNetworkControllerCreationFunctionMap[name] = func;
}

EntityController* EntityMapper::createEntityController(EntityDef& ed, Entity* e)
{
    EntityControllerCreationFunc func = _entityControllerCreationFunctionMap[ed._controller];
    assert(func != NULL);
    
    return func(e);
}

EntityNetworkController* EntityMapper::createEntityNetworkController(EntityDef& ed, Entity* e)
{
    EntityNetworkControllerCreationFunc func = _entityNetworkControllerCreationFunctionMap[ed._networkController];
    assert(func != NULL);
    
    return func(e, ed._server);
}

const std::vector<EntityDef*>& EntityMapper::getEntityDescriptors()
{
    return _entityDescriptors;
}

const std::map<std::string, EntityControllerCreationFunc>& EntityMapper::getEntityControllerMap()
{
    return _entityControllerCreationFunctionMap;
}

const std::map<std::string, EntityNetworkControllerCreationFunc>& EntityMapper::getEntityNetworkControllerMap()
{
    return _entityNetworkControllerCreationFunctionMap;
}

EntityMapper::EntityMapper()
{
    registerEntityController("Entity", EntityController::create);
    registerEntityNetworkController("Entity", EntityNetworkController::create);
}

EntityMapper::~EntityMapper()
{
    GowUtil::cleanUpMapOfPointers(_entityDescriptorsMap);
    _entityDescriptors.clear();
}
