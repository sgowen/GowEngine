//
//  TextureLoader.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void TextureLoader::loadData(Texture& t)
{
    std::string filePath = t._desc._filePath;
    
    if (ENGINE_CFG.logFileIO())
    {
        LOG("TextureLoader::loadData %s", filePath.c_str());
    }
    
    assert(t._data == nullptr);
    
    const FileData fd = ASSET_HANDLER.loadAsset(filePath);
    
    if (IS_IOS)
    {
        stbi_convert_iphone_png_to_rgb(1);
    }
    
    t._data = stbi_load_from_memory(fd._data, (int)fd._length, &t._width, &t._height, &t._numChannels, 0);
    assert(t._data != nullptr);

    ASSET_HANDLER.unloadAsset(fd);
}

void TextureLoader::freeData(Texture& t)
{
    std::string filePath = t._desc._filePath;
    
    if (ENGINE_CFG.logFileIO())
    {
        LOG("TextureLoader::freeData %s", filePath.c_str());
    }
    
    assert(t._data != nullptr);
    
    stbi_image_free(t._data);
    t._data = nullptr;
}
