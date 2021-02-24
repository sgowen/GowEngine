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
#include <algorithm>

#include <rapidjson/document.h>

void Assets::initWithJSONFile(const char* filePath)
{
    AssetHandler* ah = AssetHandlerFactory::create();
    FileData jsonData = ah->loadAsset(filePath);
    initWithJSON((const char*)jsonData._data);
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
        
        std::vector<uint16_t> soundIDsAdded;
        for (SizeType i = 0; i < v.Size(); ++i)
        {
            const Value& iv = v[i];
            assert(iv.IsObject());
            
            uint16_t soundID = RapidJSONUtil::getUInt(iv, "soundID");
            std::string filePath = RapidJSONUtil::getString(iv, "filePath");
            uint8_t numInstances = RapidJSONUtil::getUInt(iv, "numInstances");
            
            assert(std::find(soundIDsAdded.begin(), soundIDsAdded.end(), soundID) == soundIDsAdded.end());
            
            _soundDescriptors.emplace_back(soundID, filePath, numInstances);
            soundIDsAdded.emplace_back(soundID);
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
                    int count = RapidJSONUtil::getInt(iv, "count", 1);
                    
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
            std::string filterMin = RapidJSONUtil::getString(iv, "filterMin", "SHARP");
            assert(filterMin == "SHARP" || filterMin == "SMOOTH");
            std::string filterMag = RapidJSONUtil::getString(iv, "filterMag", "SHARP");
            assert(filterMag == "SHARP" || filterMag == "SMOOTH");
            bool mipMap = RapidJSONUtil::getBool(iv, "mipMap", true);
            
            _textureDescriptors.emplace_back(name, normalMapName, filePath, filterMin, filterMag, mipMap);
            
            if (iv.HasMember("mappings"))
            {
                int textureWidth = RapidJSONUtil::getInt(iv, "textureWidth", 2048);
                int textureHeight = RapidJSONUtil::getInt(iv, "textureHeight", 2048);
                
                TextureDescriptor& td = _textureDescriptors.back();
                std::map<std::string, Animation>& animations = td._animations;
                std::map<std::string, TextureRegion>& textureRegions = td._textureRegions;
                
                const Value& v = iv["mappings"];
                assert(v.IsObject());
                for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
                {
                    const Value& iv = i->value;
                    assert(iv.IsObject());
                    
                    std::string key = i->name.GetString();
                    int x = RapidJSONUtil::getInt(iv, "x");
                    int y = RapidJSONUtil::getInt(iv, "y");
                    int regionWidth = RapidJSONUtil::getInt(iv, "regionWidth");
                    int regionHeight = RapidJSONUtil::getInt(iv, "regionHeight");
                    
                    if (iv.HasMember("frameTimes") || iv.HasMember("frameTime"))
                    {
                        bool looping = RapidJSONUtil::getBool(iv, "looping", true);
                        int firstLoopingFrame = RapidJSONUtil::getInt(iv, "firstLoopingFrame");
                        int xPadding = RapidJSONUtil::getInt(iv, "xPadding");
                        int yPadding = RapidJSONUtil::getInt(iv, "yPadding");
                        
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
                            uint16_t frameTime = RapidJSONUtil::getUInt(iv, "frameTime");
                            numFrames = RapidJSONUtil::getInt(iv, "numFrames");
                            
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
                            animationWidth = RapidJSONUtil::getInt(iv, "animationWidth");
                            animationHeight = RapidJSONUtil::getInt(iv, "animationHeight");
                        }
                        
                        animations.emplace(std::piecewise_construct,
                                           std::forward_as_tuple(key),
                                           std::forward_as_tuple(x, y, regionWidth, regionHeight, animationWidth, animationHeight, textureWidth, textureHeight, looping, firstLoopingFrame, xPadding, yPadding, frameTimes)
                                           );
                    }
                    else
                    {
                        auto q = textureRegions.find(key);
                        assert(q == textureRegions.end());
                        
                        textureRegions.emplace(std::piecewise_construct,
                                               std::forward_as_tuple(key),
                                               std::forward_as_tuple(x, y, regionWidth, regionHeight, textureWidth, textureHeight)
                                               );
                    }
                }
            }
        }
    }
}

TextureRegion* Assets::findTextureRegion(std::string key, uint16_t stateTime)
{
    TextureRegion* ret = NULL;
    
    std::vector<TextureDescriptor>& tds = getTextureDescriptors();
    for (TextureDescriptor& td : tds)
    {
        ret = td.textureRegion(key, stateTime);
        if (ret != NULL)
        {
            break;
        }
    }
    
    return ret;
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
