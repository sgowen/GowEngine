//
//  EntityInputMappingsLoader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/4/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

struct EntityInputMapping;

class EntityInputMappingsLoader
{
public:
    static void loadEntityInputMapping(EntityInputMapping& eim);
    
private:
    
    EntityInputMappingsLoader();
    ~EntityInputMappingsLoader();
    EntityInputMappingsLoader(const EntityInputMappingsLoader&);
    EntityInputMappingsLoader& operator=(const EntityInputMappingsLoader&);
};
