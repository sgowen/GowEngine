//
//  TextureManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "deps/stb/TextureLoader.hpp"
#include "Texture.hpp"

#include <map>
#include <string>

class TextureManager
{
public:
    void loadData(std::map<std::string, TextureDescriptor>& tds);
    void loadIntoOpenGLAndFreeData(std::map<std::string, TextureDescriptor>& tds);
    void reset();
    Texture& texture(std::string name);
    bool isTextureLoaded(std::string name);
    
private:
    TextureLoader _loader;
    std::map<std::string, Texture> _textures;
};
