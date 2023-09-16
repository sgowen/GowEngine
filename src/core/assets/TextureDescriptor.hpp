//
//  TextureDescriptor.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/opengl/TextureRegion.hpp"
#include "core/opengl/Animation.hpp"

#include <map>
#include <string>

struct TextureDescriptor
{
    const std::string _name;
    const std::string _normalMap;
    const std::string _filePath;
    const std::string _filterMin;
    const std::string _filterMag;
    const bool _repeatS;
    const bool _mipMap;
    std::map<std::string, TextureRegion> _textureRegions;
    std::map<std::string, Animation> _animations;
    
    TextureDescriptor(std::string name, std::string normalMap, std::string filePath, std::string filterMin, std::string filterMag, bool repeatS, bool mipMap) :
    _name(name),
    _normalMap(normalMap),
    _filePath(filePath),
    _filterMin(filterMin),
    _filterMag(filterMag),
    _repeatS(repeatS),
    _mipMap(mipMap)
    {
        // Empty
    }

    TextureRegion* textureRegion(std::string key, uint16_t stateTime)
    {
        TextureRegion* ret = nullptr;
        
        const auto& qt = _textureRegions.find(key);
        if (qt != _textureRegions.end())
        {
            ret = &qt->second;
        }
        
        const auto& qa = _animations.find(key);
        if (qa != _animations.end())
        {
            ret = &qa->second.textureRegion(stateTime);
        }
        
        return ret;
    }
    
    Animation* animation(std::string key)
    {
        Animation* ret = nullptr;
        
        const auto& qa = _animations.find(key);
        if (qa != _animations.end())
        {
            ret = &qa->second;
        }
        
        return ret;
    }
};
