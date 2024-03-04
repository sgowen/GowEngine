//
//  EntityInputMappingManagerLoader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/4/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

class EntityInputMappingManager;
struct EntityInputMapping;

class EntityInputMappingManagerLoader
{
public:
    static void initWithJSONFile(EntityInputMappingManager& eimm, std::string filePath);
    static void initWithJSON(EntityInputMappingManager& eimm, const char* data);
    
    static void loadEntityInputMapping(EntityInputMapping& eld);
    
private:
    EntityInputMappingManagerLoader();
    ~EntityInputMappingManagerLoader();
    EntityInputMappingManagerLoader(const EntityInputMappingManagerLoader&);
    EntityInputMappingManagerLoader& operator=(const EntityInputMappingManagerLoader&);
};
