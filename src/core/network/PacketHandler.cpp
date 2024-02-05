//
//  PacketHandler.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

PacketHandler::PacketHandler(TimeTracker& tt, ProcessPacketFunc ppf) :
_timeTracker(tt),
_processPacketFunc(ppf),
_bytesSentThisFrame(0),
_bytesReceivedPerSecond(tt, 1.0f),
_bytesSentPerSecond(tt, 1.0f)
{
    // Empty
}

PacketHandler::~PacketHandler()
{
    // Empty
}

void PacketHandler::processIncomingPackets()
{
    readIncomingPacketsIntoQueue();
    processQueuedPackets();
    updateBytesSentLastFrame();
}

const MovingAverage& PacketHandler::bytesReceivedPerSecond() const
{
    return _bytesReceivedPerSecond;
}

const MovingAverage& PacketHandler::bytesSentPerSecond() const
{
    return _bytesSentPerSecond;
}

TimeTracker& PacketHandler::timeTracker()
{
    return _timeTracker;
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
