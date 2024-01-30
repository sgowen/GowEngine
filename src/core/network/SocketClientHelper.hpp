//
//  SocketClientHelper.h
//  noctisgames
//
//  Created by Stephen Gowen on 6/17/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#pragma once

#include "core/network/ClientHelper.hpp"

class SocketAddress;

class SocketClientHelper : public ClientHelper
{
public:
    SocketClientHelper(TimeTracker& tt, std::string serverIPAddress, std::string inName, uint16_t inPort, ProcessPacketFunc processPacketFunc);
    virtual ~SocketClientHelper();
    
    virtual int connect();
    virtual void processSpecialPacket(uint8_t packetType, InputMemoryBitStream& inInputStream, MachineAddress* inFromAddress);
    virtual void handleUninitialized();
    virtual void sendPacket(const OutputMemoryBitStream& inOutputStream);
    virtual std::string& getName();
    
private:
    SocketAddress* _serverAddress;
    std::string _name;
    uint16_t _port;
    
    void updateState();
};
