//
//  SocketPacketHandler.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/23/24.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "PacketHandler.hpp"

#include "SocketAddress.hpp"

class UDPSocket;

class SocketPacketHandler : public PacketHandler
{
public:
    SocketPacketHandler(TimeTracker& tt, uint16_t port, ProcessPacketFunc ppf);
    virtual ~SocketPacketHandler();
    
    virtual int connect() override;
    virtual void sendPacket(const OutputMemoryBitStream& ombs, MachineAddress* toAddress) override;
    
    void processIncomingPackets();
    SocketAddress& getSocketAddress();
    const MovingAverage& bytesReceivedPerSecond() const;
    const MovingAverage& bytesSentPerSecond() const;
    
protected:
    virtual void readIncomingPacketsIntoQueue() override;
    virtual void processQueuedPackets() override;
    
private:
    UDPSocket* _socket;
    SocketAddress _socketAddress;
    
    class ReceivedPacket;
    std::queue<ReceivedPacket, std::list<ReceivedPacket> > _packetQueue;
    
    class ReceivedPacket
    {
    public:
        ReceivedPacket(uint32_t receivedTime, InputMemoryBitStream& imbs, SocketAddress fromAddress);
        
        SocketAddress& getFromAddress();
        uint32_t getReceivedTime() const;
        InputMemoryBitStream& getPacketBuffer();
        
    private:
        uint32_t _receivedTime;
        InputMemoryBitStream _packetBuffer;
        SocketAddress _fromAddress;
    };
};
