//
//  EntityLayoutLoader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/27/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

class EntityIDManager;
class EntityLayout;
struct EntityLayout;

class EntityLayoutLoader
{
public:
    static void initWithJSONFile(EntityLayout& elm, std::string filePath);
    static void initWithJSON(EntityLayout& elm, const char* data);
    
    static void loadEntityLayout(EntityLayout& eld, EntityIDManager& eidm, bool isServer);
    static void saveEntityLayout(EntityLayout& eld);
    
private:
    EntityLayoutLoader();
    ~EntityLayoutLoader();
    EntityLayoutLoader(const EntityLayoutLoader&);
    EntityLayoutLoader& operator=(const EntityLayoutLoader&);
};
