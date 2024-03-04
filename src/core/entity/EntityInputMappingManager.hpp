//
//  EntityInputMappingManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/4/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

#include <map>
#include <vector>
#include <string>

struct EntityInputMappingInstance
{
    std::string _inputStateFlag;
    std::string _condition;
    
    EntityInputMappingInstance(std::string inputStateFlag, std::string condition) :
    _inputStateFlag(inputStateFlag),
    _condition(condition)
    {
        // Empty
    }
};

struct EntityInputMapping
{
    std::string _key;
    std::string _filePath;
    std::map<std::string, std::vector<EntityInputMappingInstance>> _gamepadEvents;
    std::map<std::string, std::vector<EntityInputMappingInstance>> _keyboardEvents;
    
    EntityInputMapping(std::string key = "", std::string filePath = "") :
    _key(key),
    _filePath(filePath)
    {
        // Empty
    }
};

class EntityInputMappingManager
{
    friend class EntityInputMappingManagerLoader;
    
public:
    static EntityInputMappingManager& getInstance()
    {
        static EntityInputMappingManager ret = EntityInputMappingManager();
        return ret;
    }
    
    EntityInputMapping& entityInputMapping(std::string key);
    
private:
    std::map<std::string, EntityInputMapping> _entityInputMappings;
    
    EntityInputMappingManager() {}
    ~EntityInputMappingManager() {}
    EntityInputMappingManager(const EntityInputMappingManager&);
    EntityInputMappingManager& operator=(const EntityInputMappingManager&);
};
