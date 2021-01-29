//
//  ClientHelper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/17/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "NetworkHelper.hpp"

#include <string>

#define CLIENT_NOT_READY_TO_SAY_HELLO 1
#define CLIENT_READY_TO_SAY_HELLO 2
#define CLIENT_AUTH_FAILED 3

class ClientHelper : public NetworkHelper
{
public:
    ClientHelper(PacketHandler* packetHandler);
    virtual ~ClientHelper();
    
    virtual void handleUninitialized() = 0;
    virtual void sendPacket(const OutputMemoryBitStream& outputStream) = 0;
    virtual std::string& getName() = 0;
    
    int state();
    
protected:
    int _state;
};
