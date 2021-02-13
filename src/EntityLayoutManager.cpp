//
//  EntityLayoutManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/10/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "EntityLayoutManager.hpp"

#include "Entity.hpp"
#include "EntityIDManager.hpp"

#include "StringUtil.hpp"
#include "AssetHandlerFactory.hpp"
#include "AssetHandler.hpp"
#include "FileData.hpp"
#include "PlatformMacros.hpp"
#include "Macros.hpp"
#include "RapidJSONUtil.hpp"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#include <assert.h>

void EntityLayoutManager::initWithJSONFile(const char* filePath)
{
    AssetHandler* ah = AssetHandlerFactory::create();
    FileData jsonData = ah->loadAsset(filePath);
    initWithJSON((const char*)jsonData._data);
    ah->releaseAsset(jsonData);
    AssetHandlerFactory::destroy(ah);
}

void EntityLayoutManager::initWithJSON(const char* data)
{
    clear();
    
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
        
        assert(_entityLayoutMap.find(key) == _entityLayoutMap.end());
        
        std::string filePath = iv.GetString();
        
        _entityLayoutMap.emplace(key, EntityLayoutDef{key, name, filePath});
    }
}

void EntityLayoutManager::clear()
{
    _entityLayoutMap.clear();
}

void EntityLayoutManager::loadEntityLayout(EntityLayoutDef& eld, EntityIDManager* eidm)
{
    assert(eidm != NULL);
    
    AssetHandler* ah = AssetHandlerFactory::create();
    FileData jsonData = ah->loadAsset(eld._filePath.c_str());
    
    using namespace rapidjson;
    
    Document d;
    d.Parse<kParseStopWhenDoneFlag>((const char*)jsonData._data);
    
    if (d.HasMember("entities"))
    {
        eld._entities.clear();
        eidm->resetNextLayoutEntityID();
        
        Value& v = d["entities"];
        
        assert(v.IsArray());
        for (SizeType i = 0; i < v.Size(); ++i)
        {
            const Value& iv = v[i];
            assert(iv.IsObject());
            
            std::string key = RapidJSONUtil::getString(iv, "key");
            uint32_t x = RapidJSONUtil::getUnsignedInteger(iv, "x");
            uint32_t y = RapidJSONUtil::getUnsignedInteger(iv, "y");
            
            eld._entities.emplace_back(eidm->getNextLayoutEntityID(), StringUtil::fourCharFromString(key), x, y);
        }
    }
    
    if (d.HasMember("entitiesNetwork"))
    {
        eld._entitiesNetwork.clear();
        
        Value& v = d["entitiesNetwork"];
        
        assert(v.IsArray());
        for (SizeType i = 0; i < v.Size(); ++i)
        {
            const Value& iv = v[i];
            assert(iv.IsObject());
            
            std::string key = RapidJSONUtil::getString(iv, "key");
            uint32_t x = RapidJSONUtil::getUnsignedInteger(iv, "x");
            uint32_t y = RapidJSONUtil::getUnsignedInteger(iv, "y");
            
            eld._entitiesNetwork.emplace_back(eidm->getNextNetworkEntityID(), StringUtil::fourCharFromString(key), x, y);
        }
    }
    
    ah->releaseAsset(jsonData);
    AssetHandlerFactory::destroy(ah);
}

void EntityLayoutManager::saveEntityLayout(EntityLayoutDef& eld)
{
    FILE *file = openFile(eld._filePath.c_str(), "w+");
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

std::map<uint32_t, EntityLayoutDef>& EntityLayoutManager::getEntityLayouts()
{
    return _entityLayoutMap;
}

EntityLayoutDef& EntityLayoutManager::findEntityLayoutDef(uint32_t key)
{
    auto q = _entityLayoutMap.find(key);
    if (q != _entityLayoutMap.end())
    {
        EntityLayoutDef& eld = q->second;

        return eld;
    }
    
    assert(false);
}

FILE* EntityLayoutManager::openFile(const char* path, const char* mode)
{
    FILE *file;

#if IS_WINDOWS
    errno_t err;
    if ((err = fopen_s(&file, path, mode)) != 0)
#else
    if ((file = fopen(path, mode)) == NULL)
#endif
    {
        return NULL;
    }

    return file;
}
