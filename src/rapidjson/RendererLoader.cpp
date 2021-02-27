//
//  Renderer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "RendererLoader.hpp"

#include "Renderer.hpp"
#include "AssetHandlerFactory.hpp"
#include "AssetHandler.hpp"
#include "FileData.hpp"
#include "RapidJSONUtil.hpp"

#include <assert.h>
#include <algorithm>

#include <rapidjson/document.h>

Renderer RendererLoader::initWithJSONFile(const char* filePath)
{
    FileData jsonData = ASSET_HANDLER.loadAsset(filePath);
    Renderer ret = initWithJSON((const char*)jsonData._data);
    ASSET_HANDLER.unloadAsset(jsonData);
    
    return ret;
}

Renderer RendererLoader::initWithJSON(const char* json)
{
    Renderer ret;
    
    using namespace rapidjson;
    
    Document d;
    d.Parse<kParseStopWhenDoneFlag>(json);
    assert(d.IsObject());
    
    if (d.HasMember("circleBatchers"))
    {
        Value& v = d["circleBatchers"];
        assert(v.IsObject());
        
        for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
        {
            const Value& iv = i->value;
            assert(iv.IsObject());
            
            std::string key = i->name.GetString();
            uint32_t maxBatchSize = RapidJSONUtil::getUInt(iv, "maxBatchSize");
            
            ret._circleBatchers.emplace(key, CircleBatcher{maxBatchSize});
        }
    }
    
    if (d.HasMember("fontBatchers"))
    {
        Value& v = d["fontBatchers"];
        assert(v.IsObject());
        
        for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
        {
            const Value& iv = i->value;
            assert(iv.IsObject());
            
            std::string key = i->name.GetString();
            
            std::string matrix = RapidJSONUtil::getString(iv, "matrix");
            std::string texture = RapidJSONUtil::getString(iv, "texture");
            uint32_t maxBatchSize = RapidJSONUtil::getUInt(iv, "maxBatchSize");
            uint8_t glyphsPerRow = RapidJSONUtil::getUInt(iv, "glyphsPerRow");
            uint8_t glyphWidth = RapidJSONUtil::getUInt(iv, "glyphWidth");
            uint8_t glyphHeight = RapidJSONUtil::getUInt(iv, "glyphHeight");
            
            ret._fontBatchers.emplace(key, FontBatcher{maxBatchSize, matrix, texture, glyphsPerRow, glyphWidth, glyphHeight});
        }
    }
    
    if (d.HasMember("framebuffers"))
    {
        Value& v = d["framebuffers"];
        assert(v.IsObject());
        
        for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
        {
            const Value& iv = i->value;
            assert(iv.IsObject());
            
            std::string key = i->name.GetString();
            
            uint16_t width = RapidJSONUtil::getUInt(iv, "width");
            uint16_t height = RapidJSONUtil::getUInt(iv, "height");
            std::string filterMin = RapidJSONUtil::getString(iv, "filterMin");
            std::string filterMag = RapidJSONUtil::getString(iv, "filterMag");
            
            ret._framebuffers.emplace(key, Framebuffer{width, height, filterMin, filterMag});
        }
    }
    
    if (d.HasMember("matrices"))
    {
        Value& v = d["matrices"];
        assert(v.IsObject());
        
        for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
        {
            const Value& iv = i->value;
            assert(iv.IsObject());
            
            std::string key = i->name.GetString();
            
            uint32_t width = RapidJSONUtil::getUInt(iv, "width");
            uint32_t height = RapidJSONUtil::getUInt(iv, "height");
            
            ret._matrices.emplace(key, Matrix{MatrixDescriptor(width, height)});
        }
    }
    
    if (d.HasMember("rektangleBatchers"))
    {
        Value& v = d["rektangleBatchers"];
        assert(v.IsObject());
        
        for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
        {
            const Value& iv = i->value;
            assert(iv.IsObject());
            
            std::string key = i->name.GetString();
            
            uint32_t maxBatchSize = RapidJSONUtil::getUInt(iv, "maxBatchSize");
            bool isFill = RapidJSONUtil::getBool(iv, "isFill");
            
            ret._rektangleBatchers.emplace(key, RektangleBatcher{maxBatchSize, isFill});
        }
    }
    
    if (d.HasMember("spriteBatchers"))
    {
        Value& v = d["spriteBatchers"];
        assert(v.IsObject());
        
        for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
        {
            const Value& iv = i->value;
            assert(iv.IsObject());
            
            std::string key = i->name.GetString();
            
            uint32_t maxBatchSize = RapidJSONUtil::getUInt(iv, "maxBatchSize");
            
            ret._spriteBatchers.emplace(key, SpriteBatcher{maxBatchSize});
        }
    }
    
    if (d.HasMember("textViews"))
    {
        Value& v = d["textViews"];
        assert(v.IsObject());
        
        for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
        {
            const Value& iv = i->value;
            assert(iv.IsObject());
            
            std::string key = i->name.GetString();
            
            std::string text = RapidJSONUtil::getString(iv, "text");
            uint8_t alignment = RapidJSONUtil::getUInt(iv, "alignment");
            float xWeight = RapidJSONUtil::getFloat(iv, "xWeight");
            float yWeight = RapidJSONUtil::getFloat(iv, "yWeight");
            float glyphWidthWeight = RapidJSONUtil::getFloat(iv, "glyphWidthWeight");
            
            ret._textViews.emplace(key, TextView{text, alignment, xWeight, yWeight, glyphWidthWeight});
        }
    }
    
    if (d.HasMember("triangleBatchers"))
    {
        Value& v = d["triangleBatchers"];
        assert(v.IsObject());
        
        for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
        {
            const Value& iv = i->value;
            assert(iv.IsObject());
            
            std::string key = i->name.GetString();
            
            uint32_t maxBatchSize = RapidJSONUtil::getUInt(iv, "maxBatchSize");
            bool isFill = RapidJSONUtil::getBool(iv, "isFill");
            
            ret._triangleBatchers.emplace(key, TriangleBatcher{maxBatchSize, isFill});
        }
    }
    
    return ret;
}
