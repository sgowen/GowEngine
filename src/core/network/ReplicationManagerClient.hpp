//
//  ReplicationManagerClient.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

class EntityRegistry;
class InputMemoryBitStream;

class ReplicationManagerClient
{
public:    
    void read(InputMemoryBitStream& imbs, EntityRegistry& er);
    
private:
    void readAndDoCreateAction(InputMemoryBitStream& imbs, EntityRegistry& er, uint32_t networkID);
    void readAndDoUpdateAction(InputMemoryBitStream& imbs, EntityRegistry& er, uint32_t networkID);
    void readAndDoDestroyAction(InputMemoryBitStream& imbs, EntityRegistry& er, uint32_t networkID);
};
