//
//  SocketServerHelper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/17/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "ServerHelper.hpp"

#include "PacketHandler.hpp"

class SocketServerHelper : public ServerHelper
{
public:
    SocketServerHelper(uint16_t port, ProcessPacketFunc processPacketFunc, HandleNoResponseFunc handleNoResponseFunc, HandleConnectionResetFunc handleConnectionResetFunc, GetClientProxyFunc getClientProxyFunc, HandleClientDisconnectedFunc handleClientDisconnectedFunc);
    virtual ~SocketServerHelper();
    
    virtual void processSpecialPacket(uint8_t packetType, InputMemoryBitStream& inputStream, MachineAddress* fromAddress);
    virtual void onClientDisconnected(ClientProxy* clientProxy);
    virtual MachineAddress* getServerAddress();
    virtual bool isConnected();
};
