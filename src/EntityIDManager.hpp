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
    
    uint32_t getNextLayoutEntityID();
    void resetLayoutEntityID();
    uint32_t getNextNetworkEntityID();
    
private:
    uint32_t _nextLayoutEntityID;
    uint32_t _nextNetworkEntityID;
};
