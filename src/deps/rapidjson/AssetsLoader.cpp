//
//  Assets.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <rapidjson/document.h>

void AssetsLoader::initWithJSONFile(Assets& assets, std::string filePath)
{
    FileData jsonData = ASSET_HANDLER.loadAsset(filePath);
    initWithJSON(assets, (const char*)jsonData._data);
    ASSET_HANDLER.unloadAsset(jsonData);
}

void AssetsLoader::initWithJSON(Assets& assets, const char* json)
{
    assets.reset();
    
    using namespace rapidjson;

    Document d;
    d.Parse<kParseStopWhenDoneFlag>(json);
    assert(d.IsObject());

    if (d.HasMember("entityDefs"))
    {
        Value& v = d["entityDefs"];
        assert(v.IsObject());
        
        std::string filePath = RapidJSONUtil::getString(v, "filePath");
        assets._entityDefs = new FileDescriptor(filePath, filePath);
    }
    
    if (d.HasMember("entityInputMappings"))
    {
        Value& v = d["entityInputMappings"];
        assert(v.IsObject());
        for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
        {
            const Value& v = i->value;
            assert(v.IsObject());
            
            std::string name = i->name.GetString();
            
            assert(assets._entityInputMappings.find(name) == assets._entityInputMappings.end());
            
            std::string filePath = RapidJSONUtil::getString(v, "filePath");

            assets._entityInputMappings.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(name),
                                    std::forward_as_tuple(name, filePath)
                                    );
        }
    }
    
    if (d.HasMember("entityLayouts"))
    {
        Value& v = d["entityLayouts"];
        assert(v.IsObject());
        for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
        {
            const Value& v = i->value;
            assert(v.IsObject());
            
            std::string name = i->name.GetString();
            uint32_t key = StringUtil::fourCharFromString(name);
            
            assert(assets._entityLayouts.find(key) == assets._entityLayouts.end());
            
            std::string filePath = RapidJSONUtil::getString(v, "filePath");

            assets._entityLayouts.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(key),
                                    std::forward_as_tuple(name, filePath)
                                    );
        }
    }
    
    if (d.HasMember("fonts"))
    {
        Value& v = d["fonts"];
        assert(v.IsObject());
        for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
        {
            const Value& v = i->value;
            assert(v.IsObject());
            
            std::string name = i->name.GetString();
            
            assert(assets._fonts.find(name) == assets._fonts.end());
            
            std::string texture = RapidJSONUtil::getString(v, "texture");
            uint8_t glyphsPerRow = RapidJSONUtil::getUInt(v, "glyphsPerRow");
            uint8_t glyphWidth = RapidJSONUtil::getUInt(v, "glyphWidth");
            uint8_t glyphHeight = RapidJSONUtil::getUInt(v, "glyphHeight");

            assets._fonts.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(name),
                                    std::forward_as_tuple(name, texture, glyphsPerRow, glyphWidth, glyphHeight)
                                    );
        }
    }
    
    if (d.HasMember("renderer"))
    {
        Value& v = d["renderer"];
        assert(v.IsObject());
        
        std::string filePath = RapidJSONUtil::getString(v, "filePath");
        assets._renderer = new FileDescriptor(filePath, filePath);
    }
    
    if (d.HasMember("scripts"))
    {
        const Value& v = d["scripts"];
        assert(v.IsObject());
        for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
        {
            const Value& v = i->value;
            assert(v.IsObject());
            
            std::string name = i->name.GetString();
            
            assert(assets._scripts.find(name) == assets._scripts.end());
            
            std::string filePath = RapidJSONUtil::getString(v, "filePath");
            
            assets._scripts.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(name),
                                    std::forward_as_tuple(name, filePath)
                                    );
        }
    }
    
    if (d.HasMember("shaders"))
    {
        Value& v = d["shaders"];
        assert(v.IsObject());
        for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
        {
            const Value& v = i->value;
            assert(v.IsObject());
            
            std::string name = i->name.GetString();
            
            assert(assets._shaders.find(name) == assets._shaders.end());
            
            std::string vertexShaderFilePath = RapidJSONUtil::getString(v, "vertexShaderFilePath");
            std::string fragmentShaderFilePath = RapidJSONUtil::getString(v, "fragmentShaderFilePath");

            assets._shaders.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(name),
                                    std::forward_as_tuple(name, vertexShaderFilePath, fragmentShaderFilePath)
                                    );
        }
    }
    
    if (d.HasMember("sounds"))
    {
        Value& v = d["sounds"];
        assert(v.IsObject());
        for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
        {
            const Value& v = i->value;
            assert(v.IsObject());
            
            std::string name = i->name.GetString();
            
            assert(assets._sounds.find(name) == assets._sounds.end());
            
            std::string filePath = RapidJSONUtil::getString(v, "filePath");
            uint8_t numInstances = RapidJSONUtil::getUInt(v, "numInstances", 1);

            assets._sounds.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(name),
                                    std::forward_as_tuple(name, filePath, numInstances)
                                    );
        }
    }
    
    if (d.HasMember("textures"))
    {
        Value& v = d["textures"];
        assert(v.IsObject());
        for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
        {
            const Value& v = i->value;
            assert(v.IsObject());
            
            std::string name = i->name.GetString();
            
            assert(assets._textures.find(name) == assets._textures.end());
            
            std::string normalMap = RapidJSONUtil::getString(v, "normalMap");
            std::string filePath = RapidJSONUtil::getString(v, "filePath");
            std::string filterMin = RapidJSONUtil::getString(v, "filterMin", "SHARP");
            assert(filterMin == "SHARP" || filterMin == "SMOOTH");
            std::string filterMag = RapidJSONUtil::getString(v, "filterMag", "SHARP");
            assert(filterMag == "SHARP" || filterMag == "SMOOTH");
            bool repeatS = RapidJSONUtil::getBool(v, "repeatS", false);
            bool mipMap = RapidJSONUtil::getBool(v, "mipMap", false);

            assets._textures.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(name),
                                    std::forward_as_tuple(name, normalMap, filePath, filterMin, filterMag, repeatS, mipMap)
                                    );

            if (v.HasMember("mappings"))
            {
                int textureWidth = RapidJSONUtil::getInt(v, "textureWidth", 2048);
                int textureHeight = RapidJSONUtil::getInt(v, "textureHeight", 2048);

                TextureDescriptor& td = assets._textures.find(name)->second();
                std::map<std::string, Animation>& animations = td._animations;
                std::map<std::string, TextureRegion>& textureRegions = td._textureRegions;

                const Value& v = v["mappings"];
                assert(v.IsObject());
                for (Value::ConstMemberIterator i = v.MemberBegin(); i != v.MemberEnd(); ++i)
                {
                    const Value& v = i->value;
                    assert(v.IsObject());

                    std::string key = i->name.GetString();
                    int x = RapidJSONUtil::getInt(v, "x");
                    int y = RapidJSONUtil::getInt(v, "y");
                    int regionWidth = RapidJSONUtil::getInt(v, "regionWidth");
                    int regionHeight = RapidJSONUtil::getInt(v, "regionHeight");

                    if (v.HasMember("frameTimes") || v.HasMember("frameTime"))
                    {
                        auto q = animations.find(key);
                        assert(q == animations.end());
                        
                        bool looping = RapidJSONUtil::getBool(v, "looping", true);
                        int firstLoopingFrame = RapidJSONUtil::getInt(v, "firstLoopingFrame");
                        int xPadding = RapidJSONUtil::getInt(v, "xPadding");
                        int yPadding = RapidJSONUtil::getInt(v, "yPadding");

                        std::vector<uint16_t> frameTimes;
                        size_t numFrames;

                        if (v.HasMember("frameTimes"))
                        {
                            const Value& va = v["frameTimes"];
                            assert(va.IsArray());
                            for (SizeType i = 0; i < va.Size(); ++i)
                            {
                                const Value& va = va[i];
                                frameTimes.push_back(va.GetUint());
                            }

                            numFrames = frameTimes.size();
                        }
                        else
                        {
                            uint16_t frameTime = RapidJSONUtil::getUInt(v, "frameTime");
                            numFrames = RapidJSONUtil::getUInt(v, "numFrames");

                            frameTimes.reserve(numFrames);
                            for (size_t i = 0; i < numFrames; ++i)
                            {
                                frameTimes.push_back(frameTime);
                            }
                        }

                        std::vector<uint16_t> regionWidths;
                        std::vector<uint16_t> regionHeights;

                        if (v.HasMember("regionWidths"))
                        {
                            const Value& va = v["regionWidths"];
                            assert(va.IsArray());
                            for (SizeType i = 0; i < va.Size(); ++i)
                            {
                                const Value& va = va[i];
                                regionWidths.push_back(va.GetUint());
                            }
                        }
                        else
                        {
                            regionWidths.reserve(numFrames);
                            for (size_t i = 0; i < numFrames; ++i)
                            {
                                regionWidths.push_back(regionWidth);
                            }
                        }

                        if (v.HasMember("regionHeights"))
                        {
                            const Value& va = v["regionHeights"];
                            assert(va.IsArray());
                            for (SizeType i = 0; i < va.Size(); ++i)
                            {
                                const Value& va = va[i];
                                regionHeights.push_back(va.GetUint());
                            }
                        }
                        else
                        {
                            regionHeights.reserve(numFrames);
                            for (size_t i = 0; i < numFrames; ++i)
                            {
                                regionHeights.push_back(regionHeight);
                            }
                        }

                        uint16_t animationWidth = STLUtil::sum(regionWidths);
                        uint16_t animationHeight = regionHeight;
                        if (v.HasMember("animationWidth") && v.HasMember("animationHeight"))
                        {
                            animationWidth = RapidJSONUtil::getUInt(v, "animationWidth");
                            animationHeight = RapidJSONUtil::getUInt(v, "animationHeight");
                        }

                        animations.emplace(std::piecewise_construct,
                                           std::forward_as_tuple(key),
                                           std::forward_as_tuple(x, y, regionWidths, regionHeights, animationWidth, animationHeight, textureWidth, textureHeight, looping, firstLoopingFrame, xPadding, yPadding, frameTimes)
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
