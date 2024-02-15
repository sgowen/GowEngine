//
//  ConfigLoader.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/27/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <rapidjson/document.h>

void ConfigLoader::initWithJSONFile(Config& config, std::string filePath)
{
    FileData jsonData = ASSET_HANDLER.loadAsset(filePath);
    initWithJSON(config, (const char*)jsonData._data);
    ASSET_HANDLER.unloadAsset(jsonData);
}

void ConfigLoader::initWithJSON(Config& config, const char* json)
{
    using namespace rapidjson;
    
    Document d;
    d.Parse<kParseStopWhenDoneFlag>(json);
    assert(d.IsObject());
    for (Value::ConstMemberIterator i = d.MemberBegin(); i != d.MemberEnd(); ++i)
    {
        assert(i->value.IsString());
        
        config.putString(i->name.GetString(), i->value.GetString());
    }
}
