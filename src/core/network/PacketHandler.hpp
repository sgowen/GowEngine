//
//  PacketHandler.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "MovingAverage.hpp"
#include "InputMemoryBitStream.hpp"

#include <queue>
#include <list>

struct TimeTracker;
class OutputMemoryBitStream;
class MachineAddress;

typedef void (*ProcessPacketFunc)(InputMemoryBitStream& imbs, MachineAddress* fromAddress);

class PacketHandler
{
public:
    PacketHandler(TimeTracker& tt, ProcessPacketFunc ppf);
    virtual ~PacketHandler();
    
    virtual int connect() = 0;
    virtual void sendPacket(const OutputMemoryBitStream& ombs, MachineAddress* toAddress) = 0;
    
    void processIncomingPackets();
    const MovingAverage& bytesReceivedPerSecond() const;
    const MovingAverage& bytesSentPerSecond() const;
    TimeTracker& timeTracker();
    
protected:
    TimeTracker& _timeTracker;
    ProcessPacketFunc _processPacketFunc;
    int _bytesSentThisFrame;
    
    virtual void readIncomingPacketsIntoQueue() = 0;
    virtual void processQueuedPackets() = 0;
    
    void updateBytesSentLastFrame();
    void updateBytesReceivedLastFrame(int totalReadByteCount);
    
private:
    MovingAverage _bytesReceivedPerSecond;
    MovingAverage _bytesSentPerSecond;
};
