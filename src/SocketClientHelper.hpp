//
//  SocketClientHelper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/17/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "ClientHelper.hpp"

#include "PacketHandler.hpp"

class SocketAddress;

class SocketClientHelper : public ClientHelper
{
public:
    SocketClientHelper(std::string serverIPAddress, std::string name, uint16_t port, ProcessPacketFunc processPacketFunc, HandleNoResponseFunc handleNoResponseFunc, HandleConnectionResetFunc handleConnectionResetFunc);
    virtual ~SocketClientHelper();
    
    virtual void processSpecialPacket(uint8_t packetType, InputMemoryBitStream& inputStream, MachineAddress* fromAddress);
    virtual void handleUninitialized();
    virtual void sendPacket(const OutputMemoryBitStream& outputStream);
    virtual std::string& getName();
    
private:
    SocketAddress* _serverAddress;
    std::string _name;
    
    void updateState();
};
