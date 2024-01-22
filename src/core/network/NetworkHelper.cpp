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

const MovingAverage& NetworkHelper::getBytesReceivedPerSecond() const
{
    return _packetHandler->getBytesReceivedPerSecond();
}

const MovingAverage& NetworkHelper::getBytesSentPerSecond() const
{
    return _packetHandler->getBytesSentPerSecond();
}
