//
//  Renderer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <rapidjson/document.h>

void RendererLoader::initWithJSONFile(Renderer& renderer, std::string filePath)
{
    FileData jsonData = ASSET_HANDLER.loadAsset(filePath);
    initWithJSON(renderer, (const char*)jsonData._data);
    ASSET_HANDLER.unloadAsset(jsonData);
}

void RendererLoader::initWithJSON(Renderer& renderer, const char* json)
{
    renderer.reset();
    
    using namespace rapidjson;
    
    Document d;
    d.Parse<kParseStopWhenDoneFlag>(json);
    assert(d.IsObject());
    
    if (d.HasMember("script"))
    {
        Value& v = d["script"];
        assert(v.IsString());
        
        renderer._scriptName = RapidJSONUtil::getString(d, "script");
    }
    
    if (d.HasMember("pixelToUnitRatio"))
    {
        Value& v = d["pixelToUnitRatio"];
        assert(v.IsInt());
        
        renderer._pixelToUnitRatio = RapidJSONUtil::getUInt(d, "pixelToUnitRatio");
    }
    
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
            
            renderer._circleBatchers.emplace(key, CircleBatcher{maxBatchSize});
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
            
            std::string font = RapidJSONUtil::getString(iv, "font");
            uint32_t maxBatchSize = RapidJSONUtil::getUInt(iv, "maxBatchSize");
            
            renderer._fontBatchers.emplace(key, FontBatcher{font, maxBatchSize});
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
            std::string filterMin = RapidJSONUtil::getString(iv, "filterMin", "SHARP");
            assert(filterMin == "SHARP" || filterMin == "SMOOTH");
            std::string filterMag = RapidJSONUtil::getString(iv, "filterMag", "SHARP");
            assert(filterMag == "SHARP" || filterMag == "SMOOTH");
            
            renderer._framebuffers.emplace(key, Framebuffer{width, height, filterMin, filterMag});
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
            
            renderer._imageViews.emplace(key, ImageView{texture, textureRegion, xWeight, yWeight, widthWeight, heightWeight});
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
                uint32_t w = RapidJSONUtil::getUInt(iv, "width");
                uint32_t h = RapidJSONUtil::getUInt(iv, "height");
                
                renderer._matrices.emplace(key, Matrix{MatrixDescriptor(w, h)});
            }
            else
            {
                float l = RapidJSONUtil::getFloat(iv, "left");
                float r = RapidJSONUtil::getFloat(iv, "right");
                float b = RapidJSONUtil::getFloat(iv, "bottom");
                float t = RapidJSONUtil::getFloat(iv, "top");
                float n = RapidJSONUtil::getFloat(iv, "near", -1);
                float f = RapidJSONUtil::getFloat(iv, "far", 1);
                
                renderer._matrices.emplace(key, Matrix{MatrixDescriptor(l, r, b, t, n, f)});
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
            
            renderer._rektangleBatchers.emplace(key, RektangleBatcher{maxBatchSize, isFill});
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
            
            renderer._spriteBatchers.emplace(key, SpriteBatcher{maxBatchSize});
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
            
            renderer._textViews.emplace(key, TextView{text, alignment, xWeight, yWeight, glyphWidthWeight});
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
            
            renderer._triangleBatchers.emplace(key, TriangleBatcher{maxBatchSize, isFill});
        }
    }
}
