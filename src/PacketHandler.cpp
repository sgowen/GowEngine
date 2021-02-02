//
//  PacketHandler.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "PacketHandler.hpp"

#include "TimeTracker.hpp"
#include "WeightedTimedMovingAverage.hpp"
#include "InputMemoryBitStream.hpp"
#include "OutputMemoryBitStream.hpp"
#include "MachineAddress.hpp"

PacketHandler::PacketHandler(TimeTracker* timing, bool isServer, ProcessPacketFunc processPacketFunc, HandleNoResponseFunc handleNoResponseFunc, HandleConnectionResetFunc handleConnectionResetFunc) :
_timeTracker(timing),
_processPacketFunc(processPacketFunc),
_handleNoResponseFunc(handleNoResponseFunc),
_handleConnectionResetFunc(handleConnectionResetFunc),
_bytesReceivedPerSecond(timing, 1.0f),
_bytesSentPerSecond(timing, 1.0f),
_bytesSentThisFrame(0),
_isServer(isServer)
{
    // Empty
}

void PacketHandler::processIncomingPackets()
{
    readIncomingPacketsIntoQueue();
    
    processQueuedPackets();
    
    updateBytesSentLastFrame();
}

const WeightedTimedMovingAverage& PacketHandler::getBytesReceivedPerSecond() const
{
    return _bytesReceivedPerSecond;
}

const WeightedTimedMovingAverage& PacketHandler::getBytesSentPerSecond() const
{
    return _bytesSentPerSecond;
}

void PacketHandler::updateBytesSentLastFrame()
{
    if (_bytesSentThisFrame > 0)
    {
        _bytesSentPerSecond.updatePerSecond(static_cast<float>(_bytesSentThisFrame));
        
        _bytesSentThisFrame = 0;
    }
}

void PacketHandler::updateBytesReceivedLastFrame(int totalReadByteCount)
{
    _bytesReceivedPerSecond.updatePerSecond(static_cast<float>(totalReadByteCount));
}
