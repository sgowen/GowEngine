//
//  Renderer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <rapidjson/document.h>

Renderer RendererLoader::initWithJSONFile(std::string filePath)
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
    
    if (d.HasMember("imageViews"))
    {
        Value& v = d["imageViews"];
        assert(v.IsObject());
        
        for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
        {
            const Value& iv = i->value;
            assert(iv.IsObject());
            
            std::string key = i->name.GetString();
            
            std::string texture = RapidJSONUtil::getString(iv, "texture");
            std::string textureRegion = RapidJSONUtil::getString(iv, "textureRegion");
            float xWeight = RapidJSONUtil::getFloat(iv, "xWeight");
            float yWeight = RapidJSONUtil::getFloat(iv, "yWeight");
            float widthWeight = RapidJSONUtil::getFloat(iv, "widthWeight");
            float heightWeight = RapidJSONUtil::getFloat(iv, "heightWeight");
            
            ret._imageViews.emplace(key, ImageView{texture, textureRegion, xWeight, yWeight, widthWeight, heightWeight});
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
            
            if (iv.HasMember("width"))
            {
                uint32_t width = RapidJSONUtil::getUInt(iv, "width");
                uint32_t height = RapidJSONUtil::getUInt(iv, "height");
                
                ret._matrices.emplace(key, Matrix{MatrixDescriptor(width, height)});
            }
            else
            {
                float left = RapidJSONUtil::getFloat(iv, "left");
                float right = RapidJSONUtil::getFloat(iv, "right");
                float bottom = RapidJSONUtil::getFloat(iv, "bottom");
                float top = RapidJSONUtil::getFloat(iv, "top");
                float near = RapidJSONUtil::getFloat(iv, "near", -1);
                float far = RapidJSONUtil::getFloat(iv, "far", 1);
                
                ret._matrices.emplace(key, Matrix{MatrixDescriptor(left, right, bottom, top, near, far)});
            }
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
