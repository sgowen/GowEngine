//
//  NetworkHelper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/17/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

NetworkHelper::NetworkHelper(PacketHandler* packetHandler) : _packetHandler(packetHandler)
{
    // Empty
}

NetworkHelper::~NetworkHelper()
{
    delete _packetHandler;
}

void NetworkHelper::processIncomingPackets()
{
    _packetHandler->processIncomingPackets();
}

void NetworkHelper::sendPacket(const OutputMemoryBitStream& inOutputStream, MachineAddress* inFromAddress)
{
    _packetHandler->sendPacket(inOutputStream, inFromAddress);
}

const MovingAverage& NetworkHelper::bytesReceivedPerSecond() const
{
    return _packetHandler->bytesReceivedPerSecond();
}

const MovingAverage& NetworkHelper::bytesSentPerSecond() const
{
    return _packetHandler->bytesSentPerSecond();
}

TimeTracker& NetworkHelper::timeTracker()
{
    return _packetHandler->timeTracker();
}
