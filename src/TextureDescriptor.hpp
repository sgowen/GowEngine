//
//  TextureDescriptor.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "TextureRegion.hpp"
#include "Animation.hpp"

#include <map>
#include <string>

struct TextureDescriptor
{
    const std::string _name;
    const std::string _normalMapName;
    const std::string _filePath;
    const std::string _filterMin;
    const std::string _filterMag;
    const bool _mipMap;
    std::map<std::string, TextureRegion> _textureRegions;
    std::map<std::string, Animation> _animations;
    
    TextureDescriptor(std::string name, std::string normalMapName, std::string filePath, std::string filterMin, std::string filterMag, bool mipMap) :
    _name(name),
    _normalMapName(normalMapName),
    _filePath(filePath),
    _filterMin(filterMin),
    _filterMag(filterMag),
    _mipMap(mipMap)
    {
        // Empty
    }

    TextureRegion* textureRegion(std::string key, uint16_t stateTime)
    {
        TextureRegion* ret = NULL;
        
        auto& qt = _textureRegions.find(key);
        if (qt != _textureRegions.end())
        {
            ret = &qt->second;
        }
        
        auto& qa = _animations.find(key);
        if (qa != _animations.end())
        {
            ret = &qa->second.textureRegion(stateTime);
        }
        
        return ret;
    }
};
