//
//  EntityLayoutMapper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/10/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "EntityLayoutMapper.hpp"

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

EntityLayoutMapper& EntityLayoutMapper::getInstance()
{
    static EntityLayoutMapper ret = EntityLayoutMapper();
    return ret;
}

void EntityLayoutMapper::initWithJSONFile(const char* filePath)
{
    AssetHandler* ah = AssetHandlerFactory::create();
    FileData jsonData = ah->loadAsset(filePath);
    initWithJSON(jsonData._data);
    ah->releaseAsset(jsonData);
    AssetHandlerFactory::destroy(ah);
}

void EntityLayoutMapper::initWithJSON(const char* data)
{
    _maps.clear();
    _layouts.clear();
    
    using namespace rapidjson;
    
    Document d;
    d.Parse<kParseStopWhenDoneFlag>(data);
    
    assert(d.IsObject());
    for (Value::ConstMemberIterator i = d.MemberBegin(); i != d.MemberEnd(); ++i)
    {
        const Value& iv = i->value;
        assert(iv.IsString());
        
        std::string keyStr = i->name.GetString();
        uint32_t key = StringUtil::fourCharFromString(keyStr);
        
        assert(_layouts.find(key) == _layouts.end());
        
        std::string fileName = iv.GetString();
        
        _maps.push_back(MapDef(key, keyStr, fileName));
        _layouts[key] = fileName;
    }
}

void EntityLayoutMapper::loadEntityLayout(uint32_t name, EntityIDManager* entityIDManager)
{
    std::string filePath = getJsonConfigFilePath(name);
    
    loadEntityLayout(filePath, entityIDManager);
}

void EntityLayoutMapper::loadEntityLayout(std::string filePath, EntityIDManager* entityIDManager)
{
    assert(entityIDManager);
    
    _entityIDManager = entityIDManager;
    _entityLayoutDef._entities.clear();
    
    AssetHandler* ah = AssetHandlerFactory::create();
    FileData jsonData = ah->loadAsset(filePath.c_str());
    loadEntityLayout(jsonData._data);
    ah->releaseAsset(jsonData);
    AssetHandlerFactory::destroy(ah);
}

EntityLayoutDef& EntityLayoutMapper::getEntityLayoutDef()
{
    return _entityLayoutDef;
}

std::vector<MapDef>& EntityLayoutMapper::getMaps()
{
    return _maps;
}

void EntityLayoutMapper::loadEntityLayout(const char* data)
{
    _entityIDManager->reset();
    
    using namespace rapidjson;
    
    Document d;
    d.Parse<kParseStopWhenDoneFlag>(data);
    
    if (d.HasMember("entities"))
    {
        Value& v = d["entities"];
        
        assert(v.IsArray());
        for (SizeType i = 0; i < v.Size(); ++i)
        {
            const Value& iv = v[i];
            assert(iv.IsObject());
            
            std::string keyStr = RapidJSONUtil::getString(iv, "key");
            uint32_t key = StringUtil::fourCharFromString(keyStr);
            
            uint32_t ID = _entityIDManager->getNextStaticEntityID();
            uint32_t x = RapidJSONUtil::getUnsignedInteger(iv, "x");
            uint32_t y = RapidJSONUtil::getUnsignedInteger(iv, "y");
            uint32_t w = RapidJSONUtil::getUnsignedInteger(iv, "w");
            uint32_t h = RapidJSONUtil::getUnsignedInteger(iv, "h");
            
            _entityLayoutDef._entities.emplace_back(ID, key, x, y, w, h);
        }
    }
}

void EntityLayoutMapper::saveEntityLayout(uint32_t name, EntityLayoutDef* layout)
{
    std::string filePath = getJsonConfigFilePath(name);
    
    saveEntityLayout(filePath, layout);
}

void EntityLayoutMapper::saveEntityLayout(std::string filePath, EntityLayoutDef* layout)
{
    assert(layout != NULL);
    
    using namespace rapidjson;
    
    static StringBuffer s;
    PrettyWriter<StringBuffer> w(s);
    
    s.Clear();

    w.StartObject();
    
    if (layout->_entities.size() > 0)
    {
        w.String("entities");
        w.StartArray();
        for (EntityInstanceDef eid : layout->_entities)
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
            if (eid._w > 0)
            {
                w.String("w");
                w.Uint(eid._w);
            }
            if (eid._h > 0)
            {
                w.String("h");
                w.Uint(eid._h);
            }
            w.EndObject();
        }
        w.EndArray();
    }
    
    w.EndObject();
    
    const char* data = s.GetString();
    
    FILE *file = openFile(filePath.c_str(), "w+");
    if (file)
    {
        int sum = fprintf(file, "%s", data);
        UNUSED(sum);

        fclose(file);
    }
}

std::string EntityLayoutMapper::getJsonConfigFilePath(uint32_t fourCCName)
{
    if (fourCCName == 'TEST')
    {
        return std::string("test.json");
    }
    
    auto q = _layouts.find(fourCCName);
    
    assert(q != _layouts.end());
    
    return q->second;
}

FILE* EntityLayoutMapper::openFile(const char* path, const char* mode)
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
