//
//  EntityLayoutLoader.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/27/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

EntityLayout EntityLayoutLoader::initWithJSONFile(std::string filePath)
{
    FileData fd = ASSET_HANDLER.loadAsset(filePath);
    EntityLayout ret = initWithJSON((const char*)fd._data);
    ASSET_HANDLER.unloadAsset(fd);
    
    return ret;
}

EntityLayout EntityLayoutLoader::initWithJSON(const char* data)
{
    EntityLayout ret;
    
    using namespace rapidjson;
    
    Document d;
    d.Parse<kParseStopWhenDoneFlag>(data);
    
    assert(d.IsObject());
    for (Value::ConstMemberIterator i = d.MemberBegin(); i != d.MemberEnd(); ++i)
    {
        const Value& iv = i->value;
        assert(iv.IsString());
        
        std::string name = i->name.GetString();
        uint32_t key = StringUtil::fourCharFromString(name);
        
        assert(ret._entityLayouts.find(key) == ret._entityLayouts.end());
        
        std::string filePath = iv.GetString();
        
        ret._entityLayouts.emplace(key, EntityLayoutDef{key, name, filePath});
    }
    
    return ret;
}

void EntityLayoutLoader::loadEntityLayout(EntityLayoutDef& eld, bool isServer)
{
    EntityIDManager* eidm = INST_REG.get<EntityIDManager>(isServer ? INSK_EID_SRVR : INSK_EID_CLNT);
    assert(eidm != NULL);
    eidm->resetNextLayoutEntityID();
    
    eld._entities.clear();
    eld._entitiesNetwork.clear();
    
    FileData fd = ASSET_HANDLER.loadAsset(eld._filePath);
    
    using namespace rapidjson;
    
    Document d;
    d.Parse<kParseStopWhenDoneFlag>((const char*)fd._data);
    
    if (d.HasMember("entities"))
    {
        Value& v = d["entities"];
        assert(v.IsArray());
        
        for (SizeType i = 0; i < v.Size(); ++i)
        {
            const Value& iv = v[i];
            assert(iv.IsObject());
            
            std::string key = RapidJSONUtil::getString(iv, "key");
            uint32_t x = RapidJSONUtil::getUInt(iv, "x");
            uint32_t y = RapidJSONUtil::getUInt(iv, "y");
            
            eld._entities.emplace_back(eidm->getNextLayoutEntityID(), StringUtil::fourCharFromString(key), x, y, isServer);
        }
    }
    
    if (d.HasMember("entitiesNetwork"))
    {
        Value& v = d["entitiesNetwork"];
        assert(v.IsArray());
        
        for (SizeType i = 0; i < v.Size(); ++i)
        {
            const Value& iv = v[i];
            assert(iv.IsObject());
            
            std::string key = RapidJSONUtil::getString(iv, "key");
            uint32_t x = RapidJSONUtil::getUInt(iv, "x");
            uint32_t y = RapidJSONUtil::getUInt(iv, "y");
            
            eld._entitiesNetwork.emplace_back(eidm->getNextNetworkEntityID(), StringUtil::fourCharFromString(key), x, y, isServer);
        }
    }
    
    ASSET_HANDLER.unloadAsset(fd);
}

void EntityLayoutLoader::saveEntityLayout(EntityLayoutDef& eld)
{
    FILE *file = OPEN_FILE(eld._filePath.c_str(), "w+");
    if (file == NULL)
    {
        return;
    }
    
    using namespace rapidjson;
    
    static StringBuffer s;
    PrettyWriter<StringBuffer> w(s);
    
    s.Clear();

    w.StartObject();
    
    if (eld._entities.size() > 0)
    {
        w.String("entities");
        w.StartArray();
        for (auto& eid : eld._entities)
        {
            w.StartObject();
            {
                w.String("key");
                std::string key = StringUtil::stringFromFourChar(eid._key);
                w.String(key.c_str());
            }
            {
                w.String("x");
                w.Uint(eid._x);
            }
            {
                w.String("y");
                w.Uint(eid._y);
            }
            w.EndObject();
        }
        w.EndArray();
    }
    
    w.EndObject();
    
    const char* data = s.GetString();
    
    int sum = fprintf(file, "%s", data);
    UNUSED(sum);

    fclose(file);
}
