//
//  EmbeddedAssetHandler.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/16/24.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <GowEngine/embeddedData/engine_assets_json.h>
#include <GowEngine/embeddedData/engine_config_json.h>

#include <GowEngine/embeddedData/framebuffer_fsh.h>
#include <GowEngine/embeddedData/framebuffer_vsh.h>
#include <GowEngine/embeddedData/geometry_fsh.h>
#include <GowEngine/embeddedData/geometry_vsh.h>
#include <GowEngine/embeddedData/lights_fsh.h>
#include <GowEngine/embeddedData/lights_vsh.h>
#include <GowEngine/embeddedData/shockwave_fsh.h>
#include <GowEngine/embeddedData/shockwave_vsh.h>
#include <GowEngine/embeddedData/sprite_fsh.h>
#include <GowEngine/embeddedData/sprite_vsh.h>

#include <GowEngine/embeddedData/sf_font_png.h>

FileData EmbeddedAssetHandler::loadAsset(std::string filePath)
{
    std::vector<uint8_t>* blob = new std::vector<uint8_t>();
    
    if (StringUtil::doesStringEndWithString(filePath, "data/json/Engine/Assets.json"))
    {
        blob->insert(blob->begin(), std::begin(engine_assets_json), std::end(engine_assets_json));
    }
    else if (StringUtil::doesStringEndWithString(filePath, "data/json/Engine/Config.json"))
    {
        blob->insert(blob->begin(), std::begin(engine_config_json), std::end(engine_config_json));
    }
    else if (StringUtil::doesStringEndWithString(filePath, "data/shaders/framebuffer.fsh"))
    {
        blob->insert(blob->begin(), std::begin(framebuffer_fsh), std::end(framebuffer_fsh));
    }
    else if (StringUtil::doesStringEndWithString(filePath, "data/shaders/framebuffer.vsh"))
    {
        blob->insert(blob->begin(), std::begin(framebuffer_vsh), std::end(framebuffer_vsh));
    }
    else if (StringUtil::doesStringEndWithString(filePath, "data/shaders/geometry.fsh"))
    {
        blob->insert(blob->begin(), std::begin(geometry_fsh), std::end(geometry_fsh));
    }
    else if (StringUtil::doesStringEndWithString(filePath, "data/shaders/geometry.vsh"))
    {
        blob->insert(blob->begin(), std::begin(geometry_vsh), std::end(geometry_vsh));
    }
    else if (StringUtil::doesStringEndWithString(filePath, "data/shaders/lights.fsh"))
    {
        blob->insert(blob->begin(), std::begin(lights_fsh), std::end(lights_fsh));
    }
    else if (StringUtil::doesStringEndWithString(filePath, "data/shaders/lights.vsh"))
    {
        blob->insert(blob->begin(), std::begin(lights_vsh), std::end(lights_vsh));
    }
    else if (StringUtil::doesStringEndWithString(filePath, "data/shaders/shockwave.fsh"))
    {
        blob->insert(blob->begin(), std::begin(shockwave_fsh), std::end(shockwave_fsh));
    }
    else if (StringUtil::doesStringEndWithString(filePath, "data/shaders/shockwave.vsh"))
    {
        blob->insert(blob->begin(), std::begin(shockwave_vsh), std::end(shockwave_vsh));
    }
    else if (StringUtil::doesStringEndWithString(filePath, "data/shaders/sprite.fsh"))
    {
        blob->insert(blob->begin(), std::begin(sprite_fsh), std::end(sprite_fsh));
    }
    else if (StringUtil::doesStringEndWithString(filePath, "data/shaders/sprite.vsh"))
    {
        blob->insert(blob->begin(), std::begin(sprite_vsh), std::end(sprite_vsh));
    }
    else if (StringUtil::doesStringEndWithString(filePath, "data/textures/sf_font.png"))
    {
        blob->insert(blob->begin(), std::begin(sf_font_png), std::end(sf_font_png));
    }

    return FileData(static_cast<long>(blob->size()), blob->data(), (const void*)blob);
}

void EmbeddedAssetHandler::unloadAsset(const FileData& fileData)
{
    assert(fileData._fileHandle != nullptr);

    const std::vector<uint8_t>* handle = static_cast<const std::vector<uint8_t>*>(fileData._fileHandle);
    delete handle;
}

bool EmbeddedAssetHandler::isAssetEmbedded(std::string filePath)
{
    if (StringUtil::doesStringEndWithString(filePath, "data/json/Engine/Config.json"))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, "data/json/Engine/Assets.json"))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, "data/shaders/framebuffer.vsh"))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, "data/shaders/framebuffer.fsh"))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, "data/shaders/geometry.vsh"))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, "data/shaders/geometry.fsh"))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, "data/shaders/lights.vsh"))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, "data/shaders/lights.fsh"))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, "data/shaders/shockwave.vsh"))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, "data/shaders/shockwave.fsh"))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, "data/shaders/sprite.vsh"))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, "data/shaders/sprite.fsh"))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, "data/textures/sf_font.png"))
    {
        return true;
    }
    
    return false;
}
