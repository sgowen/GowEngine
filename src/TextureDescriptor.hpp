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

class TextureDescriptor
{
public:
    const std::string _name;
    const std::string _normalMapName;
    const std::string _filePath;
    const std::string _filterMin;
    const std::string _filterMag;
    const bool _mipMap;
    
    TextureDescriptor(std::string name, std::string normalMapName, std::string filePath, std::string filterMin, std::string filterMag, bool mipMap);
    
    TextureRegion& textureRegion(std::string key, uint16_t stateTime);
    TextureRegion& textureRegion(std::string key);
    Animation& animation(std::string key);
    std::map<std::string, TextureRegion>& getTextureRegions();
    std::map<std::string, Animation>& getAnimations();
    
private:
    std::map<std::string, TextureRegion> _textureRegions;
    std::map<std::string, Animation> _animations;
};
