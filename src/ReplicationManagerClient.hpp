//
//  ReplicationManagerClient.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

class EntityRegistry;
class InputMemoryBitStream;

class ReplicationManagerClient
{
public:
    ReplicationManagerClient(EntityRegistry& entityRegistry);
    
    void read(InputMemoryBitStream& imbs);
    
private:
    EntityRegistry& _entityRegistry;
    
    void readAndDoCreateAction(InputMemoryBitStream& imbs, uint32_t networkID);
    void readAndDoUpdateAction(InputMemoryBitStream& imbs, uint32_t networkID);
    void readAndDoDestroyAction(InputMemoryBitStream& imbs, uint32_t networkID);
};
