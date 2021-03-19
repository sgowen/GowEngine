//
//  EntityLayoutLoader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/27/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/entity/EntityLayout.hpp"

class EntityLayoutLoader
{
public:
    static EntityLayout initWithJSONFile(std::string filePath);
    static EntityLayout initWithJSON(const char* data);
    static void loadEntityLayout(EntityLayoutDef& eld, bool isServer);
    static void saveEntityLayout(EntityLayoutDef& eld);
    
private:
    EntityLayoutLoader();
    ~EntityLayoutLoader();
    EntityLayoutLoader(const EntityLayoutLoader&);
    EntityLayoutLoader& operator=(const EntityLayoutLoader&);
};
