//
//  EntityLayoutLoader.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/27/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

void EntityLayoutLoader::loadEntityLayout(EntityLayout& el, EntityIDManager& eidm, bool isServer)
{
    eidm.resetNextLayoutEntityID();
    
    el._entities.clear();
    el._entitiesNetwork.clear();
    
    FileData fd = ASSET_HANDLER.loadAsset(el._filePath);
    
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
            
            el._entities.emplace_back(eidm.getNextLayoutEntityID(), StringUtil::fourCharFromString(key), x, y, isServer);
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
            
            el._entitiesNetwork.emplace_back(eidm.getNextNetworkEntityID(), StringUtil::fourCharFromString(key), x, y, isServer);
        }
    }
    
    ASSET_HANDLER.unloadAsset(fd);
}

void EntityLayoutLoader::saveEntityLayout(EntityLayout& el)
{
    std::string resFilePath = RES_FILE_PATH(el._filePath);
    
    FILE* file = OPEN_FILE(resFilePath, "w+");
    if (file == nullptr)
    {
        return;
    }
    
    using namespace rapidjson;
    
    static StringBuffer s;
    PrettyWriter<StringBuffer> w(s);
    
    s.Clear();

    w.StartObject();
    
    if (el._entities.size() > 0)
    {
        w.String("entities");
        w.StartArray();
        for (auto& eid : el._entities)
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
