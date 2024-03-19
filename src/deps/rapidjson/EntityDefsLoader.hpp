//
//  EntityDefsLoader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/26/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>

struct EntityDef;

class EntityDefsLoader
{
public:
    static void initWithJSONFile(std::map<uint32_t, EntityDef>& entityDefs, std::string filePath);
    static void initWithJSON(std::map<uint32_t, EntityDef>& entityDefs, const char* json);
    
private:
    EntityDefsLoader();
    ~EntityDefsLoader();
    EntityDefsLoader(const EntityDefsLoader&);
    EntityDefsLoader& operator=(const EntityDefsLoader&);
};
