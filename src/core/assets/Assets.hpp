//
//  Assets.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Font.hpp"
#include "ShaderDescriptor.hpp"
#include "SoundDescriptor.hpp"
#include "TextureDescriptor.hpp"

#include <vector>

struct Assets
{
    std::vector<Font> _fonts;
    std::vector<ShaderDescriptor> _shaderDescriptors;
    std::vector<SoundDescriptor> _soundDescriptors;
    std::vector<TextureDescriptor> _textureDescriptors;
    
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
