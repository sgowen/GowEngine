//
//  EntityManagerLoader.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/26/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <rapidjson/document.h>

void EntityManagerLoader::initWithJSONFile(EntityManager& em, std::string filePath)
{
    FileData fd = ASSET_HANDLER.loadAsset(filePath);
    initWithJSON(em, (const char*)fd._data);
    ASSET_HANDLER.unloadAsset(fd);
}

void EntityManagerLoader::initWithJSON(EntityManager& em, const char* json)
{
    em._entityDefs.clear();
    
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
        
        assert(em._entityDefs.find(key) == em._entityDefs.end());
        
        std::string name = RapidJSONUtil::getString(iv, "name");
        std::string keyName = keyStr;
        std::string controller = RapidJSONUtil::getString(iv, "controller", "Default");
        std::string networkController = RapidJSONUtil::getString(iv, "networkController", "Default");
        std::string renderController = RapidJSONUtil::getString(iv, "renderController", "Default");
        
        std::map<uint8_t, std::string> textureMappings;
        if (iv.HasMember("textureMappings"))
        {
            const Value& v = iv["textureMappings"];
            assert(v.IsObject());
            for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
            {
                std::string name = i->name.GetString();
                uint16_t nameVal = StringUtil::stringToNumber<uint16_t>(name);
                uint8_t state = (uint8_t)nameVal;
                const Value& value = i->value;
                assert(value.IsString());
                std::string stateMapping = value.GetString();
                textureMappings.emplace(state, stateMapping);
            }
        }
        else if (iv.HasMember("textureMapping"))
        {
            const Value& v = iv["textureMapping"];
            assert(v.IsString());
            uint8_t state = 0;
            std::string stateMapping = v.GetString();
            textureMappings.emplace(state, stateMapping);
        }
        
        std::map<uint8_t, std::vector<SoundMapping> > soundMappings;
        if (iv.HasMember("soundMappings"))
        {
            const Value& v = iv["soundMappings"];
            assert(v.IsObject());
            for (Value::ConstMemberIterator member = v.MemberBegin(); member != v.MemberEnd(); ++member)
            {
                std::string name = member->name.GetString();
                uint16_t nameVal = StringUtil::stringToNumber<uint16_t>(name);
                uint8_t state = (uint8_t)nameVal;
                assert(member->value.IsString() || member->value.IsArray());
                
                std::vector<SoundMapping> stateSoundMappings;
                if (member->value.IsString())
                {
                    std::string soundID = member->value.GetString();
                    stateSoundMappings.emplace_back(0, soundID);
                }
                else
                {
                    for (SizeType j = 0; j < member->value.Size(); ++j)
                    {
                        const Value& jv = member->value[j];
                        assert(jv.IsObject());
                        
                        uint16_t stateTime = RapidJSONUtil::getUInt(jv, "stateTime");
                        std::string soundID = RapidJSONUtil::getString(jv, "soundID");
                        
                        stateSoundMappings.emplace_back(stateTime, soundID);
                    }
                }
                
                soundMappings.emplace(state, stateSoundMappings);
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
                
                fixtureDef._halfWidthFactor = RapidJSONUtil::getFloat(iv, "halfWidthFactor", 0.5f);
                fixtureDef._halfHeightFactor = RapidJSONUtil::getFloat(iv, "halfHeightFactor", 0.5f);
                fixtureDef._flags = RapidJSONUtil::getUInt(iv, "flags");
                fixtureDef._restitution = RapidJSONUtil::getFloat(iv, "restitution");
                fixtureDef._density = RapidJSONUtil::getFloat(iv, "density");
                fixtureDef._friction = RapidJSONUtil::getFloat(iv, "friction");
                
                fixtures.push_back(fixtureDef);
            }
        }
        uint8_t bodyFlags = RapidJSONUtil::getUInt(iv, "bodyFlags");
        uint8_t width = RapidJSONUtil::getUInt(iv, "width");
        uint8_t height = RapidJSONUtil::getUInt(iv, "height");
        float scale = RapidJSONUtil::getFloat(iv, "scale", 1.0f);
        
        Config data;
        if (iv.HasMember("data"))
        {
            const Value& v = iv["data"];
            assert(v.IsObject());
            for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
            {
                assert(i->value.IsString());
                
                data.putString(i->name.GetString(), i->value.GetString());
            }
        }
        
        std::vector<NetworkDataGroup> networkDataGroups;
        if (iv.HasMember("networkData"))
        {
            const Value& v = iv["networkData"];
            assert(v.IsArray());
            assert(v.Size() <= 6);
            for (SizeType i = 0; i < v.Size(); ++i)
            {
                const Value& iv = v[i];
                assert(iv.IsObject());
                
                uint8_t readStateFlag = RSTF_EXTRA_DATA_BEGIN << i;
                std::string name = RapidJSONUtil::getString(iv, "name");
                
                std::vector<NetworkDataField> networkDataFields;
                assert(iv.HasMember("data"));
                const Value& dataValue = iv["data"];
                assert(dataValue.IsArray());
                for (SizeType i = 0; i < dataValue.Size(); ++i)
                {
                    const Value& iv = dataValue[i];
                    assert(iv.IsObject());
                    
                    std::string name = RapidJSONUtil::getString(iv, "name");
                    uint8_t type = RapidJSONUtil::getUInt(iv, "type");
                    std::string value = RapidJSONUtil::getString(iv, "value");
                    
                    networkDataFields.emplace_back(name, type, value);
                }
                
                networkDataGroups.emplace_back(readStateFlag, name, networkDataFields);
            }
        }
        NetworkData nd(networkDataGroups);
        
        em._entityDefs.emplace(key, EntityDef{key, name, keyName, controller, networkController, renderController, textureMappings, soundMappings, fixtures, bodyFlags, width, height, scale, data, nd});
    }
}
