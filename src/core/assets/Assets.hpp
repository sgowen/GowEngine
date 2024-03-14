//
//  Assets.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Font.hpp"
#include "ScriptDescriptor.hpp"
#include "ShaderDescriptor.hpp"
#include "SoundDescriptor.hpp"
#include "TextureDescriptor.hpp"

#include <vector>

struct Assets
{
    std::vector<Font> _fonts;
    std::map<std::string, EntityInputMappingDescriptor> _entityInputMappingDescriptors;
    std::map<std::string, EntityLayoutDescriptor> _entityLayoutDescriptors;
    std::map<std::string, ScriptDescriptor> _scriptDescriptors;
    std::vector<ShaderDescriptor> _shaderDescriptors;
    std::vector<SoundDescriptor> _soundDescriptors;
    std::vector<TextureDescriptor> _textureDescriptors;
    bool _isDataLoaded;
    bool _isLoadedIntoEngine;
    
    Assets() : _isDataLoaded(false), _isLoadedIntoEngine(false)
    {
        // Empty
    }
    
    void reset()
    {
        _fonts.clear();
        _entityInputMappingDescriptors.clear();
        _entityLayoutDescriptors.clear();
        _scriptDescriptors.clear();
        _shaderDescriptors.clear();
        _soundDescriptors.clear();
        _textureDescriptors.clear();
        _isDataLoaded = false;
        _isLoadedIntoEngine = false;
    }
    
    ScriptDescriptor* script(std::string key)
    {
        ScriptDescriptor* ret = nullptr;
        
        const auto& q = _scriptDescriptors.find(key);
        if (q != _scriptDescriptors.end())
        {
            ret = &q->second;
        }
        
        return ret;
    }
    
    Font* font(std::string key)
    {
        Font* ret = nullptr;
        
        for (Font& f : _fonts)
        {
            if (f._name == key)
            {
                ret = &f;
            }
        }
        
        return ret;
    }
    
    std::string textureForRegionKey(std::string key)
    {
        std::vector<TextureDescriptor>& tds = _textureDescriptors;
        for (TextureDescriptor& td : tds)
        {
            TextureRegion* tr = td.textureRegion(key, 0);
            if (tr != nullptr)
            {
                return td._name;
            }
        }
        
        return "";
    }
    
    TextureRegion* textureRegion(std::string key, uint16_t stateTime)
    {
        TextureRegion* ret = nullptr;
        
        std::vector<TextureDescriptor>& tds = _textureDescriptors;
        for (TextureDescriptor& td : tds)
        {
            ret = td.textureRegion(key, stateTime);
            if (ret != nullptr)
            {
                break;
            }
        }
        
        return ret;
    }
    
    Animation* animation(std::string key)
    {
        Animation* ret = nullptr;
        
        std::vector<TextureDescriptor>& tds = _textureDescriptors;
        for (TextureDescriptor& td : tds)
        {
            ret = td.animation(key);
            if (ret != nullptr)
            {
                break;
            }
        }
        
        return ret;
    }
};
