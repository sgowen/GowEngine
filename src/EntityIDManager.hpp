//
//  EntityIDManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/28/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

class EntityIDManager
{
public:
    EntityIDManager();
    ~EntityIDManager() {}
    
    uint32_t getNextStaticEntityID();
    void resetStaticEntityID();
    uint32_t getNextDynamicEntityID();
    
private:
    uint32_t _staticEntityID;
    uint32_t _dynamicEntityID;
};
