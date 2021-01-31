//
//  EntityLayoutMapper.hpp
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
    uint32_t _entityID;
    uint32_t _key;
    uint32_t _x;
    uint32_t _y;
    uint32_t _w;
    uint32_t _h;
    
    EntityInstanceDef(uint32_t entityID, uint32_t key, uint32_t x = 0, uint32_t y = 0, uint32_t w = 0, uint32_t h = 0) :
    _entityID(entityID),
    _key(key),
    _x(x),
    _y(y),
    _w(w),
    _h(h)
    {
        // Empty
    }
};

struct EntityLayoutDef
{
    std::vector<EntityInstanceDef> _entities;
};

struct MapDef
{
    uint32_t _key;
    std::string _name;
    std::string _value;
    
    MapDef(uint32_t key, std::string name, std::string value) :
    _key(key),
    _name(name),
    _value(value)
    {
        // Empty
    }
};

class EntityIDManager;

#define ENTITY_LAYOUT_MAPPER EntityLayoutMapper::getInstance()

class EntityLayoutMapper
{
public:
    static EntityLayoutMapper& getInstance();
    
    void initWithJSONFile(const char* filePath);
    void initWithJSON(const char* data);
    void clear();
    void loadEntityLayout(uint32_t name, EntityIDManager* entityIDManager);
    void loadEntityLayout(std::string filePath, EntityIDManager* entityIDManager);
    void saveEntityLayout(uint32_t name, EntityLayoutDef* layout);
    void saveEntityLayout(std::string filePath, EntityLayoutDef* layout);
    EntityLayoutDef& getEntityLayoutDef();
    std::vector<MapDef>& getMaps();
    std::string getJsonConfigFilePath(uint32_t fourCCName);
    
private:
    std::vector<MapDef> _maps;
    std::map<uint32_t, std::string> _layouts;
    EntityLayoutDef _entityLayoutDef;
    EntityIDManager* _entityIDManager;
    
    void loadEntityLayout(const char* data);
    FILE* openFile(const char* path, const char* mode);
    
    EntityLayoutMapper() {}
    ~EntityLayoutMapper() {}
    EntityLayoutMapper(const EntityLayoutMapper&);
    EntityLayoutMapper& operator=(const EntityLayoutMapper&);
};
