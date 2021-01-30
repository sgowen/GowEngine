//
//  Assets.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "Assets.hpp"

#include "AssetHandlerFactory.hpp"
#include "AssetHandler.hpp"
#include "FileData.hpp"
#include "RapidJSONUtil.hpp"

#include <assert.h>

#include <rapidjson/document.h>

Assets& Assets::getInstance()
{
    static Assets ret = Assets();
    return ret;
}

void Assets::initWithJSONFile(const char* filePath)
{
    AssetHandler* ah = AssetHandlerFactory::create();
    FileData jsonData = ah->loadAsset(filePath);
    initWithJSON(jsonData._data);
    ah->releaseAsset(jsonData);
    AssetHandlerFactory::destroy(ah);
}

void Assets::initWithJSON(const char* json)
{
    _shaderDescriptors.clear();
    _soundDescriptors.clear();
    _textureDescriptors.clear();
    
    using namespace rapidjson;
    
    Document d;
    d.Parse<kParseStopWhenDoneFlag>(json);
    
    if (d.HasMember("music"))
    {
        Value& v = d["music"];
        
        assert(v.IsObject());
        std::string filePath = RapidJSONUtil::getString(v, "filePath");
        
        _soundDescriptors.emplace_back(1337, filePath, 1);
    }
    
    if (d.HasMember("sounds"))
    {
        Value& v = d["sounds"];
        assert(v.IsArray());
        
        for (SizeType i = 0; i < v.Size(); ++i)
        {
            const Value& iv = v[i];
            assert(iv.IsObject());
            
            uint16_t soundID = RapidJSONUtil::getInteger(iv, "soundID");
            std::string filePath = RapidJSONUtil::getString(iv, "filePath");
            int numInstances = RapidJSONUtil::getInteger(iv, "numInstances");
            
            _soundDescriptors.emplace_back(soundID, filePath, numInstances);
        }
    }
    
    if (d.HasMember("shaders"))
    {
        Value& v = d["shaders"];
        assert(v.IsArray());
        
        for (SizeType i = 0; i < v.Size(); ++i)
        {
            const Value& iv = v[i];
            assert(iv.IsObject());
            
            std::string name = RapidJSONUtil::getString(iv, "name");
            std::string vertexShaderFilePath = RapidJSONUtil::getString(iv, "vertexShaderFilePath");
            std::string fragmentShaderFilePath = RapidJSONUtil::getString(iv, "fragmentShaderFilePath");
            
            _shaderDescriptors.emplace_back(name, vertexShaderFilePath, fragmentShaderFilePath);
            
            if (iv.HasMember("uniforms"))
            {
                ShaderDescriptor& sd = _shaderDescriptors.back();
                std::vector<ShaderUniform>& uniforms = sd.getUniforms();
                
                const Value& v = iv["uniforms"];
                assert(v.IsArray());
                for (SizeType i = 0; i < v.Size(); ++i)
                {
                    const Value& iv = v[i];
                    std::string name = RapidJSONUtil::getString(iv, "name");
                    std::string type = RapidJSONUtil::getString(iv, "type");
                    
                    uniforms.emplace_back(name, type);
                }
            }
            
            if (iv.HasMember("attributes"))
            {
                ShaderDescriptor& sd = _shaderDescriptors.back();
                std::vector<ShaderAttribute>& attributes = sd.getAttributes();
                
                const Value& v = iv["attributes"];
                assert(v.IsArray());
                for (SizeType i = 0; i < v.Size(); ++i)
                {
                    const Value& iv = v[i];
                    std::string name = RapidJSONUtil::getString(iv, "name");
                    std::string type = RapidJSONUtil::getString(iv, "type");
                    int count = RapidJSONUtil::getInteger(iv, "count", 1);
                    
                    attributes.emplace_back(name, type, count);
                }
            }
        }
    }
    
    if (d.HasMember("textures"))
    {
        Value& v = d["textures"];
        assert(v.IsArray());
        
        for (SizeType i = 0; i < v.Size(); ++i)
        {
            const Value& iv = v[i];
            assert(iv.IsObject());
            
            std::string name = RapidJSONUtil::getString(iv, "name");
            std::string normalMapName = RapidJSONUtil::getString(iv, "normalMapName");
            std::string filePath = RapidJSONUtil::getString(iv, "filePath");
            std::string filterMin = RapidJSONUtil::getString(iv, "filterMin", "NEAREST");
            std::string filterMag = RapidJSONUtil::getString(iv, "filterMag", "NEAREST");
            bool mipMap = RapidJSONUtil::getBool(iv, "mipMap", true);
            int layer = RapidJSONUtil::getInteger(iv, "layer");
            
            _textureDescriptors.emplace_back(name, normalMapName, filePath, filterMin, filterMag, mipMap, layer);
            
            if (iv.HasMember("mappings"))
            {
                int textureWidth = RapidJSONUtil::getInteger(iv, "textureWidth", 2048);
                int textureHeight = RapidJSONUtil::getInteger(iv, "textureHeight", 2048);
                
                TextureDescriptor& td = _textureDescriptors.back();
                std::map<std::string, Animation>& animations = td.getAnimations();
                std::map<std::string, TextureRegion>& textureRegions = td.getTextureRegions();
                
                const Value& v = iv["mappings"];
                assert(v.IsObject());
                for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
                {
                    const Value& iv = i->value;
                    assert(iv.IsObject());
                    
                    std::string key = i->name.GetString();
                    int x = RapidJSONUtil::getInteger(iv, "x");
                    int y = RapidJSONUtil::getInteger(iv, "y");
                    int regionWidth = RapidJSONUtil::getInteger(iv, "regionWidth");
                    int regionHeight = RapidJSONUtil::getInteger(iv, "regionHeight");
                    
                    if (iv.HasMember("frameTimes") || iv.HasMember("frameTime"))
                    {
                        bool looping = RapidJSONUtil::getBool(iv, "looping", true);
                        int firstLoopingFrame = RapidJSONUtil::getInteger(iv, "firstLoopingFrame");
                        int xPadding = RapidJSONUtil::getInteger(iv, "xPadding");
                        int yPadding = RapidJSONUtil::getInteger(iv, "yPadding");
                        
                        auto q = animations.find(key);
                        assert(q == animations.end());
                        
                        std::vector<uint16_t> frameTimes;
                        int numFrames;
                        
                        if (iv.HasMember("frameTimes"))
                        {
                            const Value& va = iv["frameTimes"];
                            assert(va.IsArray());
                            for (SizeType i = 0; i < va.Size(); ++i)
                            {
                                const Value& iva = va[i];
                                frameTimes.push_back(iva.GetInt());
                            }
                            
                            numFrames = static_cast<int>(frameTimes.size());
                        }
                        else
                        {
                            uint16_t frameTime = RapidJSONUtil::getInteger(iv, "frameTime");
                            numFrames = RapidJSONUtil::getInteger(iv, "numFrames");
                            
                            frameTimes.reserve(numFrames);
                            for (int i = 0; i < numFrames; ++i)
                            {
                                frameTimes.push_back(frameTime);
                            }
                        }
                        
                        int animationWidth = regionWidth * numFrames;
                        int animationHeight = regionHeight;
                        if (iv.HasMember("animationWidth") && iv.HasMember("animationHeight"))
                        {
                            animationWidth = RapidJSONUtil::getInteger(iv, "animationWidth");
                            animationHeight = RapidJSONUtil::getInteger(iv, "animationHeight");
                        }
                        
                        animations.insert({key, Animation(x, y, regionWidth, regionHeight, animationWidth, animationHeight, textureWidth, textureHeight, looping, firstLoopingFrame, xPadding, yPadding, frameTimes, layer)});
                    }
                    else
                    {
                        auto q = textureRegions.find(key);
                        assert(q == textureRegions.end());
                        
                        textureRegions.insert({key, TextureRegion(x, y, regionWidth, regionHeight, textureWidth, textureHeight, layer)});
                    }
                }
            }
        }
    }
}

TextureRegion& Assets::findTextureRegion(std::string key, uint16_t stateTime)
{
    std::vector<TextureDescriptor>& tds = getTextureDescriptors();
    for (TextureDescriptor& td : tds)
    {
        {
            std::map<std::string, TextureRegion>& textureRegions = td.getTextureRegions();
            auto q = textureRegions.find(key);
            if (q != textureRegions.end())
            {
                TextureRegion& tr = q->second;

                return tr;
            }
        }
        
        {
            std::map<std::string, Animation>& animations = td.getAnimations();
            auto q = animations.find(key);
            if (q != animations.end())
            {
                Animation& a = q->second;

                return a.getTextureRegion(stateTime);
            }
        }
    }
    
    assert(false);
}

TextureDescriptor& Assets::findTextureDescriptor(std::string key)
{
    std::vector<TextureDescriptor>& tds = getTextureDescriptors();
    for (TextureDescriptor& td : tds)
    {
        {
            std::map<std::string, TextureRegion>& textureRegions = td.getTextureRegions();
            auto q = textureRegions.find(key);
            if (q != textureRegions.end())
            {
                return td;
            }
        }
        
        {
            std::map<std::string, Animation>& animations = td.getAnimations();
            auto q = animations.find(key);
            if (q != animations.end())
            {
                return td;
            }
        }
    }
    
    assert(false);
}

std::vector<ShaderDescriptor>& Assets::getShaderDescriptors()
{
    return _shaderDescriptors;
}

std::vector<SoundDescriptor>& Assets::getSoundDescriptors()
{
    return _soundDescriptors;
}

std::vector<TextureDescriptor>& Assets::getTextureDescriptors()
{
    return _textureDescriptors;
}
