//
//  PacketHandler.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "WeightedTimedMovingAverage.hpp"

#include <queue>
#include <list>

class TimeTracker;
class InputMemoryBitStream;
class OutputMemoryBitStream;
class MachineAddress;

typedef void (*ProcessPacketFunc)(InputMemoryBitStream& inputStream, MachineAddress* fromAddress);
typedef void (*HandleNoResponseFunc)();
typedef void (*HandleConnectionResetFunc)(MachineAddress* fromAddress);

class PacketHandler
{
public:
    PacketHandler(TimeTracker* timing, bool isServer, ProcessPacketFunc processPacketFunc, HandleNoResponseFunc handleNoResponseFunc, HandleConnectionResetFunc handleConnectionResetFunc);
    virtual ~PacketHandler() {}
    
    virtual void sendPacket(const OutputMemoryBitStream& outputStream, MachineAddress* fromAddress) = 0;
    
    void processIncomingPackets();
    const WeightedTimedMovingAverage& getBytesReceivedPerSecond() const;
    const WeightedTimedMovingAverage& getBytesSentPerSecond() const;
    
protected:
    TimeTracker* _timeTracker;
    ProcessPacketFunc _processPacketFunc;
    HandleNoResponseFunc _handleNoResponseFunc;
    HandleConnectionResetFunc _handleConnectionResetFunc;
    int _bytesSentThisFrame;
    bool _isServer;
    
    virtual void readIncomingPacketsIntoQueue() = 0;
    virtual void processQueuedPackets() = 0;
    
    void updateBytesSentLastFrame();
    void updateBytesReceivedLastFrame(int totalReadByteCount);
    
private:
    WeightedTimedMovingAverage _bytesReceivedPerSecond;
    WeightedTimedMovingAverage _bytesSentPerSecond;
};
