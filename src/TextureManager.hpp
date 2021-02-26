//
//  TextureManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "stb/TextureLoader.hpp"
#include "Texture.hpp"

#include <map>
#include <string>

class TextureManager
{
public:
    void loadTextures(std::vector<TextureDescriptor>& tds);
    void unloadTextures(std::vector<TextureDescriptor>& tds);
    Texture& texture(std::string name);
    
private:
    TextureLoader _loader;
    std::map<std::string, Texture> _textures;
};
