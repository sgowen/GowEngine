//
//  Assets.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Font.hpp"
#include "FileDescriptor.hpp"
#include "ShaderDescriptor.hpp"
#include "SoundDescriptor.hpp"
#include "TextureDescriptor.hpp"

struct Assets
{
    FileDescriptor* _entityDefs;
    std::map<std::string, FileDescriptor> _entityInputMappings;
    std::map<uint32_t, FileDescriptor> _entityLayouts;
    std::map<std::string, Font> _fonts;
    FileDescriptor* _renderer;
    std::map<std::string, FileDescriptor> _scripts;
    std::map<std::string, ShaderDescriptor> _shaders;
    std::map<std::string, SoundDescriptor> _sounds;
    std::map<std::string, TextureDescriptor> _textures;
    bool _isDataLoaded;
    bool _isLoadedIntoEngine;
    
    Assets() :
    _entityDefs(nullptr),
    _renderer(nullptr),
    _isDataLoaded(false),
    _isLoadedIntoEngine(false)
    {
        // Empty
    }
    
    bool needsToLoadDescriptors()
    {
        return
        _entityDefs == nullptr &&
        _entityInputMappings.empty() &&
        _entityLayouts.empty() &&
        _fonts.empty() &&
        _renderer == nullptr &&
        _scripts.empty() &&
        _shaders.empty() &&
        _sounds.empty() &&
        _textures.empty();
    }
    
    void reset()
    {
        if (_entityDefs != nullptr)
        {
            delete _entityDefs;
            _entityDefs = nullptr;
        }
        
        _entityInputMappings.clear();
        _entityLayouts.clear();
        _fonts.clear();
        
        if (_renderer != nullptr)
        {
            delete _renderer;
            _renderer = nullptr;
        }
        
        _scripts.clear();
        _shaders.clear();
        _sounds.clear();
        _textures.clear();
        
        _isDataLoaded = false;
        _isLoadedIntoEngine = false;
    }
    
    FileDescriptor* script(std::string key)
    {
        FileDescriptor* ret = nullptr;
        
        const auto& q = _scripts.find(key);
        if (q != _scripts.end())
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
        std::vector<TextureDescriptor>& tds = _textures;
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
        
        std::vector<TextureDescriptor>& tds = _textures;
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
        
        std::vector<TextureDescriptor>& tds = _textures;
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
