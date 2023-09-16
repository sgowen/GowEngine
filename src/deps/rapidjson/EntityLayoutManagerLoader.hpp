//
//  EntityLayoutManagerLoader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/27/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

class EntityIDManager;
class EntityLayoutManager;
struct EntityLayoutDef;

class EntityLayoutManagerLoader
{
public:
    static void initWithJSONFile(EntityLayoutManager& elm, std::string filePath);
    static void initWithJSON(EntityLayoutManager& elm, const char* data);
    
    static void loadEntityLayout(EntityLayoutDef& eld, EntityIDManager& eidm, bool isServer);
    static void saveEntityLayout(EntityLayoutDef& eld);
    
private:
    EntityLayoutManagerLoader();
    ~EntityLayoutManagerLoader();
    EntityLayoutManagerLoader(const EntityLayoutManagerLoader&);
    EntityLayoutManagerLoader& operator=(const EntityLayoutManagerLoader&);
};
