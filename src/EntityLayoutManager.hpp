//
//  EntityLayoutManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/10/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
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
    
    EntityInstanceDef(uint32_t ID, uint32_t key, uint32_t x = 0, uint32_t y = 0) :
    _ID(ID),
    _key(key),
    _x(x),
    _y(y)
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

class EntityIDManager;

#define ENTITY_LAYOUT_MGR EntityLayoutManager::getInstance()

class EntityLayoutManager
{
public:
    static EntityLayoutManager& getInstance()
    {
        static EntityLayoutManager ret = EntityLayoutManager();
        return ret;
    }
    
    void initWithJSONFile(const char* filePath);
    void initWithJSON(const char* data);
    void clear();
    void loadEntityLayout(EntityLayoutDef& eld, EntityIDManager* entityIDManager);
    void saveEntityLayout(EntityLayoutDef& eld);
    std::map<uint32_t, EntityLayoutDef>& getEntityLayouts();
    EntityLayoutDef& findEntityLayoutDef(uint32_t key);
    
private:
    std::map<uint32_t, EntityLayoutDef> _entityLayoutMap;
    
    FILE* openFile(const char* path, const char* mode);
    
    EntityLayoutManager() {}
    ~EntityLayoutManager() {}
    EntityLayoutManager(const EntityLayoutManager&);
    EntityLayoutManager& operator=(const EntityLayoutManager&);
};
