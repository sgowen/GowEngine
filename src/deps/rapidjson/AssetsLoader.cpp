//
//  Assets.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <rapidjson/document.h>

Assets AssetsLoader::initWithJSONFile(std::string filePath)
{
    FileData jsonData = ASSET_HANDLER.loadAsset(filePath);
    Assets ret = initWithJSON((const char*)jsonData._data);
    ASSET_HANDLER.unloadAsset(jsonData);

    return ret;
}

Assets AssetsLoader::initWithJSON(const char* json)
{
    Assets ret;

    using namespace rapidjson;

    Document d;
    d.Parse<kParseStopWhenDoneFlag>(json);
    assert(d.IsObject());

    std::vector<std::string> soundIDsAdded;
    if (d.HasMember("music"))
    {
        Value& v = d["music"];
        assert(v.IsObject());

        std::string filePath = RapidJSONUtil::getString(v, "filePath");
        std::string soundID = "music";
        ret._soundDescriptors.emplace_back(soundID, filePath, 1);
        soundIDsAdded.emplace_back(soundID);
    }

    if (d.HasMember("sounds"))
    {
        Value& v = d["sounds"];
        assert(v.IsArray());
        for (SizeType i = 0; i < v.Size(); ++i)
        {
            const Value& iv = v[i];
            assert(iv.IsObject());

            std::string soundID = RapidJSONUtil::getString(iv, "soundID");
            std::string filePath = RapidJSONUtil::getString(iv, "filePath");
            uint8_t numInstances = RapidJSONUtil::getUInt(iv, "numInstances", 1);

            assert(std::find(soundIDsAdded.begin(), soundIDsAdded.end(), soundID) == soundIDsAdded.end());

            ret._soundDescriptors.emplace_back(soundID, filePath, numInstances);
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

            ret._shaderDescriptors.emplace_back(name, vertexShaderFilePath, fragmentShaderFilePath);

            if (iv.HasMember("uniforms"))
            {
                ShaderDescriptor& sd = ret._shaderDescriptors.back();
                std::vector<ShaderUniform>& uniforms = sd._uniforms;

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
                ShaderDescriptor& sd = ret._shaderDescriptors.back();
                std::vector<ShaderAttribute>& attributes = sd._attributes;

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
    
    if (d.HasMember("fonts"))
    {
        Value& v = d["fonts"];
        assert(v.IsArray());
        for (SizeType i = 0; i < v.Size(); ++i)
        {
            const Value& iv = v[i];
            assert(iv.IsObject());

            std::string name = RapidJSONUtil::getString(iv, "name");
            std::string texture = RapidJSONUtil::getString(iv, "texture");
            uint8_t glyphsPerRow = RapidJSONUtil::getUInt(iv, "glyphsPerRow");
            uint8_t glyphWidth = RapidJSONUtil::getUInt(iv, "glyphWidth");
            uint8_t glyphHeight = RapidJSONUtil::getUInt(iv, "glyphHeight");

            ret._fonts.emplace_back(name, texture, glyphsPerRow, glyphWidth, glyphHeight);
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
            std::string normalMap = RapidJSONUtil::getString(iv, "normalMap");
            std::string filePath = RapidJSONUtil::getString(iv, "filePath");
            std::string filterMin = RapidJSONUtil::getString(iv, "filterMin", "SHARP");
            assert(filterMin == "SHARP" || filterMin == "SMOOTH");
            std::string filterMag = RapidJSONUtil::getString(iv, "filterMag", "SHARP");
            assert(filterMag == "SHARP" || filterMag == "SMOOTH");
            bool repeatS = RapidJSONUtil::getBool(iv, "repeatS", false);
            bool mipMap = RapidJSONUtil::getBool(iv, "mipMap", false);

            ret._textureDescriptors.emplace_back(name, normalMap, filePath, filterMin, filterMag, repeatS, mipMap);

            if (iv.HasMember("mappings"))
            {
                int textureWidth = RapidJSONUtil::getInt(iv, "textureWidth", 2048);
                int textureHeight = RapidJSONUtil::getInt(iv, "textureHeight", 2048);

                TextureDescriptor& td = ret._textureDescriptors.back();
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
                        auto q = animations.find(key);
                        assert(q == animations.end());
                        
                        bool looping = RapidJSONUtil::getBool(iv, "looping", true);
                        int firstLoopingFrame = RapidJSONUtil::getInt(iv, "firstLoopingFrame");
                        int xPadding = RapidJSONUtil::getInt(iv, "xPadding");
                        int yPadding = RapidJSONUtil::getInt(iv, "yPadding");

                        std::vector<uint16_t> frameTimes;
                        size_t numFrames;

                        if (iv.HasMember("frameTimes"))
                        {
                            const Value& va = iv["frameTimes"];
                            assert(va.IsArray());
                            for (SizeType i = 0; i < va.Size(); ++i)
                            {
                                const Value& iva = va[i];
                                frameTimes.push_back(iva.GetUint());
                            }

                            numFrames = frameTimes.size();
                        }
                        else
                        {
                            uint16_t frameTime = RapidJSONUtil::getUInt(iv, "frameTime");
                            numFrames = RapidJSONUtil::getUInt(iv, "numFrames");

                            frameTimes.reserve(numFrames);
                            for (size_t i = 0; i < numFrames; ++i)
                            {
                                frameTimes.push_back(frameTime);
                            }
                        }

                        std::vector<uint16_t> regionWidths;
                        std::vector<uint16_t> regionHeights;

                        if (iv.HasMember("regionWidths"))
                        {
                            const Value& va = iv["regionWidths"];
                            assert(va.IsArray());
                            for (SizeType i = 0; i < va.Size(); ++i)
                            {
                                const Value& iva = va[i];
                                regionWidths.push_back(iva.GetUint());
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

                        if (iv.HasMember("regionHeights"))
                        {
                            const Value& va = iv["regionHeights"];
                            assert(va.IsArray());
                            for (SizeType i = 0; i < va.Size(); ++i)
                            {
                                const Value& iva = va[i];
                                regionHeights.push_back(iva.GetUint());
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
                        if (iv.HasMember("animationWidth") && iv.HasMember("animationHeight"))
                        {
                            animationWidth = RapidJSONUtil::getUInt(iv, "animationWidth");
                            animationHeight = RapidJSONUtil::getUInt(iv, "animationHeight");
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

    return ret;
}
