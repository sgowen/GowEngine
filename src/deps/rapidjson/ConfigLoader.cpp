//
//  ConfigLoader.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/27/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <rapidjson/document.h>

Config ConfigLoader::initWithJSONFile(std::string filePath)
{
    FileData jsonData = ASSET_HANDLER.loadAsset(filePath);
    Config ret = initWithJSON((const char*)jsonData._data);
    ASSET_HANDLER.unloadAsset(jsonData);
    
    return ret;
}

Config ConfigLoader::initWithJSON(const char* json)
{
    Config ret;
    
    using namespace rapidjson;
    
    Document d;
    d.Parse<kParseStopWhenDoneFlag>(json);
    assert(d.IsObject());
    for (Value::ConstMemberIterator i = d.MemberBegin(); i != d.MemberEnd(); ++i)
    {
        assert(i->value.IsString());
        
        ret.putString(i->name.GetString(), i->value.GetString());
    }
    
    return ret;
}
