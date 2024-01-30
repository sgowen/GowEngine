//
//  SocketServerHelper.h
//  noctisgames
//
//  Created by Stephen Gowen on 6/17/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#pragma once

#include "core/network/ServerHelper.hpp"

class SocketServerHelper : public ServerHelper
{
public:
    SocketServerHelper(TimeTracker& tt, uint16_t inPort, ProcessPacketFunc inProcessPacketFunc, GetClientProxyFunc inGetClientProxyFunc, HandleClientDisconnectedFunc inHandleClientDisconnectedFunc);
    virtual ~SocketServerHelper();
    
    virtual int connect();
    virtual void processSpecialPacket(uint8_t packetType, InputMemoryBitStream& inInputStream, MachineAddress* inFromAddress);
    virtual void onClientDisconnected(ClientProxy* clientProxy);
    virtual MachineAddress* getServerAddress();
    virtual bool isConnected();
    
private:
    uint16_t _port;
};
