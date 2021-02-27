//
//  EntityManagerLoader.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/26/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "EntityManagerLoader.hpp"

#include "Entity.hpp"
#include "EntityManager.hpp"
#include "EntityLayout.hpp"
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

void EntityManagerLoader::initWithJSONFile(EntityManager& em, const char* filePath)
{
    FileData jsonData = ASSET_HANDLER.loadAsset(filePath);
    initWithJSON(em, (const char*)jsonData._data);
    ASSET_HANDLER.unloadAsset(jsonData);
}

void EntityManagerLoader::initWithJSON(EntityManager& em, const char* json)
{
    em._entityDescriptorsMap.clear();
    
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
        
        assert(em._entityDescriptorsMap.find(key) == em._entityDescriptorsMap.end());
        
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
            const Value& v = iv["data"];
            assert(v.IsObject());
            for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
            {
                assert(i->value.IsString());
                
                cfg._keyValues[i->name.GetString()] = i->value.GetString();
            }
        }
        
        em._entityDescriptorsMap.emplace(key, EntityDef{key, name, keyName, controller, networkController, physicsController, renderController, textureMappings, soundMappings, soundRandomMappings, fixtures, bodyFlags, width, height, cfg});
    }
}
