//
//  EntityNetworkController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/17/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

#define DECL_EntityNetworkController_create \
public: \
    static EntityNetworkController* create(Entity* e, bool isServer)

#define IMPL_EntityNetworkController_create(name)                \
EntityNetworkController* name::create(Entity* e, bool isServer)  \
{                                                                \
    return new name(e, isServer);                                \
}                                                                \

class Entity;
class InputMemoryBitStream;
class OutputMemoryBitStream;

class EntityNetworkController
{
    DECL_EntityNetworkController_create;
    
public:
    EntityNetworkController(Entity* e, bool isServer);
    virtual ~EntityNetworkController();
    
    virtual void read(InputMemoryBitStream& ip);
    virtual uint16_t write(OutputMemoryBitStream& op, uint16_t dirtyState);
    virtual void recallNetworkCache();
    virtual uint16_t getDirtyState();
    
    bool isServer();
    
protected:
    Entity* _entity;
    bool _isServer;
};
