//
//  SocketPacketHandler.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "PacketHandler.hpp"

#include "InputMemoryBitStream.hpp"
#include "SocketAddress.hpp"

#include <queue>
#include <list>

class OutputMemoryBitStream;
class UDPSocket;
class MachineAddress;

class SocketPacketHandler : public PacketHandler
{
public:
    SocketPacketHandler(Timing* timing, bool isServer, uint16_t port, ProcessPacketFunc processPacketFunc, HandleNoResponseFunc handleNoResponseFunc, HandleConnectionResetFunc handleConnectionResetFunc);
    virtual ~SocketPacketHandler();
    
    virtual void sendPacket(const OutputMemoryBitStream& outputStream, MachineAddress* fromAddress);
    
    SocketAddress* getSocketAddress();
    bool isInitialized();
    
protected:
    virtual void readIncomingPacketsIntoQueue();
    virtual void processQueuedPackets();
    
private:
    class ReceivedPacket;
    
    SocketAddress* _socketAddress;
    UDPSocket* _socket;
    std::queue<ReceivedPacket, std::list<ReceivedPacket> > _packetQueue;
    bool _isInitialized;
    
    class ReceivedPacket
    {
    public:
        ReceivedPacket(float receivedTime, InputMemoryBitStream& inputMemoryBitStream, SocketAddress fromAddress);
        
        SocketAddress& getFromAddress();
        float getReceivedTime()	const;
        InputMemoryBitStream& getPacketBuffer();
        
    private:
        float _receivedTime;
        InputMemoryBitStream _packetBuffer;
        SocketAddress _fromAddress;
    };
};
