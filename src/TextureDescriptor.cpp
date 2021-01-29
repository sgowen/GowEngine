//
//  TextureDescriptor.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "TextureDescriptor.hpp"

#include <assert.h>

TextureDescriptor::TextureDescriptor(std::string name, std::string normalMapName, std::string filePath, std::string filterMin, std::string filterMag, bool mipMap, int layer) :
_name(name),
_normalMapName(normalMapName),
_filePath(filePath),
_filterMin(filterMin),
_filterMag(filterMag),
_mipMap(mipMap),
_layer(layer)
{
    // Empty
}

TextureRegion& TextureDescriptor::textureRegion(std::string key, uint16_t stateTime)
{
    Animation& anim = animation(key);

    return anim.getTextureRegion(stateTime);
}

TextureRegion& TextureDescriptor::textureRegion(std::string key)
{
    auto q = _textureRegions.find(key);
    assert(q != _textureRegions.end());

    return q->second;
}

Animation& TextureDescriptor::animation(std::string key)
{
    auto q = _animations.find(key);
    assert(q != _animations.end());

    return q->second;
}

std::map<std::string, TextureRegion>& TextureDescriptor::getTextureRegions()
{
    return _textureRegions;
}

std::map<std::string, Animation>& TextureDescriptor::getAnimations()
{
    return _animations;
}
