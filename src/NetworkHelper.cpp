//
//  NetworkHelper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/17/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "NetworkHelper.hpp"

#include "OutputMemoryBitStream.hpp"
#include "MachineAddress.hpp"
#include "WeightedTimedMovingAverage.hpp"
#include "PacketHandler.hpp"

#include "InputMemoryBitStream.hpp"

NetworkHelper::NetworkHelper(PacketHandler* packetHandler) :
_packetHandler(packetHandler)
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

void NetworkHelper::sendPacket(const OutputMemoryBitStream& outputStream, MachineAddress* fromAddress)
{
    _packetHandler->sendPacket(outputStream, fromAddress);
}

const WeightedTimedMovingAverage& NetworkHelper::getBytesReceivedPerSecond() const
{
    return _packetHandler->getBytesReceivedPerSecond();
}

const WeightedTimedMovingAverage& NetworkHelper::getBytesSentPerSecond() const
{
    return _packetHandler->getBytesSentPerSecond();
}
