//
//  EntityManagerLoader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/26/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>

class EntityManager;

class EntityManagerLoader
{
public:
    static void initWithJSONFile(EntityManager& em, std::string filePath);
    static void initWithJSON(EntityManager& em, const char* json);
    
private:
    EntityManagerLoader();
    ~EntityManagerLoader();
    EntityManagerLoader(const EntityManagerLoader&);
    EntityManagerLoader& operator=(const EntityManagerLoader&);
};
