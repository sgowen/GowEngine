//
//  EntityLayoutManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/10/18.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <map>
#include <vector>
#include <string>

struct EntityInstanceDef
{
    uint32_t _ID;
    uint32_t _key;
    uint32_t _x;
    uint32_t _y;
    bool _isServer;
    
    EntityInstanceDef(uint32_t ID, uint32_t key, uint32_t x, uint32_t y, bool isServer) :
    _ID(ID),
    _key(key),
    _x(x),
    _y(y),
    _isServer(isServer)
    {
        // Empty
    }
};

struct EntityLayoutDef
{
    uint32_t _key;
    std::string _name;
    std::string _filePath;
    std::vector<EntityInstanceDef> _entities;
    std::vector<EntityInstanceDef> _entitiesNetwork;
    
    EntityLayoutDef(uint32_t key = 0, std::string name = "", std::string filePath = "") :
    _key(key),
    _name(name),
    _filePath(filePath)
    {
        // Empty
    }
};

class EntityLayoutManager
{
    friend class EntityLayoutManagerLoader;
    
public:
    static EntityLayoutManager& getInstance()
    {
        static EntityLayoutManager ret = EntityLayoutManager();
        return ret;
    }
    
    EntityLayoutDef& entityLayoutDef(uint32_t key);
    uint32_t getFirstLayout();
    
private:
    std::map<uint32_t, EntityLayoutDef> _entityLayouts;
    
    EntityLayoutManager();
    ~EntityLayoutManager() {}
    EntityLayoutManager(const EntityLayoutManager&);
    EntityLayoutManager& operator=(const EntityLayoutManager&);
};
