//
//  ReplicationManagerClient.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

class EntityManager;
class InputMemoryBitStream;

class ReplicationManagerClient
{
public:
    ReplicationManagerClient(EntityManager* entityManager);
    
    void read(InputMemoryBitStream& inputStream);
    
private:
    EntityManager* _entityManager;
    
    void readAndDoCreateAction(InputMemoryBitStream& inputStream, uint32_t networkID);
    void readAndDoUpdateAction(InputMemoryBitStream& inputStream, uint32_t networkID);
    void readAndDoDestroyAction(InputMemoryBitStream& inputStream, uint32_t networkID);
};
