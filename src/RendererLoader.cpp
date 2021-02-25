//
//  Renderer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "RendererLoader.hpp"

#include "Renderer.hpp"
#include "AssetHandlerFactory.hpp"
#include "AssetHandler.hpp"
#include "FileData.hpp"
#include "RapidJSONUtil.hpp"

#include <assert.h>
#include <algorithm>

#include <rapidjson/document.h>

Renderer RendererLoader::initWithJSONFile(const char* filePath)
{
    AssetHandler* ah = AssetHandlerFactory::create();
    FileData jsonData = ah->loadAsset(filePath);
    Renderer ret = initWithJSON((const char*)jsonData._data);
    ah->releaseAsset(jsonData);
    AssetHandlerFactory::destroy(ah);
    
    return ret;
}

Renderer RendererLoader::initWithJSON(const char* json)
{
    Renderer ret;
    
    using namespace rapidjson;
    
    Document d;
    d.Parse<kParseStopWhenDoneFlag>(json);
    assert(d.IsObject());
    
    if (d.HasMember("circleBatchers"))
    {
        Value& v = d["circleBatchers"];
        
        // TODO
    }
    
    if (d.HasMember("fontBatchers"))
    {
        Value& v = d["fontBatchers"];
        
        // TODO
    }
    
    if (d.HasMember("framebuffers"))
    {
        Value& v = d["framebuffers"];
        
        // TODO
    }
    
    if (d.HasMember("matrices"))
    {
        Value& v = d["matrices"];
        
        // TODO
    }
    
    if (d.HasMember("rektangleBatchers"))
    {
        Value& v = d["rektangleBatchers"];
        
        // TODO
    }
    
    if (d.HasMember("spriteBatchers"))
    {
        Value& v = d["spriteBatchers"];
        
        // TODO
    }
    
    if (d.HasMember("textViews"))
    {
        Value& v = d["textViews"];
        
        // TODO
    }
    
    if (d.HasMember("triangleBatchers"))
    {
        Value& v = d["triangleBatchers"];
        
        // TODO
    }
    
    return ret;
}
