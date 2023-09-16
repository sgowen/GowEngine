//
//  EntityIDManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/28/18.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

class EntityIDManager
{
public:
    EntityIDManager();
    
    uint32_t getNextLayoutEntityID();
    void resetNextLayoutEntityID();
    uint32_t getNextNetworkEntityID();
    void resetNextNetworkEntityID();
    uint32_t getNextPlayerEntityID();
    void resetNextPlayerEntityID();
    
private:
    uint32_t _nextLayoutEntityID;
    uint32_t _nextNetworkEntityID;
    uint32_t _nextPlayerEntityID;
};
