//
//  EntityInputMappingsLoader.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/4/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

void EntityInputMappingsLoader::loadEntityInputMapping(EntityInputMapping& eim)
{
    eim._gamepadEvents.clear();
    eim._keyboardEvents.clear();
    
    FileData fd = ASSET_HANDLER.loadAsset(eim._filePath);
    
    using namespace rapidjson;
    
    Document d;
    d.Parse<kParseStopWhenDoneFlag>((const char*)fd._data);
    assert(d.IsObject());
    
    if (d.HasMember("gamepadEvents"))
    {
        Value& v = d["gamepadEvents"];
        assert(v.IsObject());
        
        for (Value::ConstMemberIterator itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr)
        {
            const Value& iv = itr->value;
            assert(iv.IsArray());
            
            std::string key = itr->name.GetString();
            assert(eim._gamepadEvents.find(key) == eim._gamepadEvents.end());
            
            std::vector<EntityInputMappingInstance> mappings;
            for (SizeType i = 0; i < iv.Size(); ++i)
            {
                const Value& iiv = iv[i];
                assert(iiv.IsObject());

                std::string inputStateFlag = RapidJSONUtil::getString(iiv, "inputStateFlag");
                std::string condition = RapidJSONUtil::getString(iiv, "condition");

                mappings.emplace_back(inputStateFlag, condition);
            }
            
            eim._gamepadEvents.emplace(key, mappings);
        }
    }
    
    if (d.HasMember("keyboardEvents"))
    {
        Value& v = d["keyboardEvents"];
        assert(v.IsObject());
        
        for (Value::ConstMemberIterator itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr)
        {
            const Value& v = itr->value;
            assert(v.IsArray());
            
            std::string key = itr->name.GetString();
            assert(eim._keyboardEvents.find(key) == eim._keyboardEvents.end());
            
            std::vector<EntityInputMappingInstance> mappings;
            for (SizeType i = 0; i < v.Size(); ++i)
            {
                const Value& iv = v[i];
                assert(iv.IsObject());

                std::string inputStateFlag = RapidJSONUtil::getString(iv, "inputStateFlag");
                std::string condition = RapidJSONUtil::getString(iv, "condition");

                mappings.emplace_back(inputStateFlag, condition);
            }
            
            eim._keyboardEvents.emplace(key, mappings);
        }
    }
    
    ASSET_HANDLER.unloadAsset(fd);
}
