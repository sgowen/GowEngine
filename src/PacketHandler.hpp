//
//  PacketHandler.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "WeightedTimedMovingAverage.hpp"
#include "InputMemoryBitStream.hpp"
#include "SocketAddress.hpp"

#include <queue>
#include <list>

class TimeTracker;
class InputMemoryBitStream;
class OutputMemoryBitStream;
class SocketAddress;
class UDPSocket;

typedef void (*ProcessPacketFunc)(InputMemoryBitStream& imbs, SocketAddress* fromAddress);
typedef void (*HandleNoResponseFunc)();
typedef void (*HandleConnectionResetFunc)(SocketAddress* fromAddress);

class PacketHandler
{
public:
    PacketHandler(TimeTracker* tt, bool isServer, uint16_t port, ProcessPacketFunc processPacketFunc, HandleNoResponseFunc handleNoResponseFunc, HandleConnectionResetFunc handleConnectionResetFunc);
    ~PacketHandler();
    
    void sendPacket(const OutputMemoryBitStream& ombs, SocketAddress* fromAddress);
    bool isConnected();
    SocketAddress& getSocketAddress();
    
    void processIncomingPackets();
    const WeightedTimedMovingAverage& getBytesReceivedPerSecond() const;
    const WeightedTimedMovingAverage& getBytesSentPerSecond() const;
    
private:
    class ReceivedPacket
    {
    public:
        ReceivedPacket(uint32_t receivedTime, InputMemoryBitStream& inputMemoryBitStream, SocketAddress fromAddress);
        
        SocketAddress& getFromAddress();
        uint32_t getReceivedTime()    const;
        InputMemoryBitStream& getPacketBuffer();
        
    private:
        uint32_t _receivedTime;
        InputMemoryBitStream _packetBuffer;
        SocketAddress _fromAddress;
    };
    
    UDPSocket* _socket;
    SocketAddress _socketAddress;
    std::queue<ReceivedPacket, std::list<ReceivedPacket> > _packetQueue;
    bool _isConnected;
    TimeTracker* _timeTracker;
    ProcessPacketFunc _processPacketFunc;
    HandleNoResponseFunc _handleNoResponseFunc;
    HandleConnectionResetFunc _handleConnectionResetFunc;
    int _bytesSentThisFrame;
    bool _isServer;
    
    void readIncomingPacketsIntoQueue();
    void processQueuedPackets();
    void updateBytesSentLastFrame();
    void updateBytesReceivedLastFrame(int totalReadByteCount);
    
private:
    WeightedTimedMovingAverage _bytesReceivedPerSecond;
    WeightedTimedMovingAverage _bytesSentPerSecond;
};
