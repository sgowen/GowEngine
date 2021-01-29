//
//  NetworkHelper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/17/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

class InputMemoryBitStream;
class OutputMemoryBitStream;
class MachineAddress;
class WeightedTimedMovingAverage;
class PacketHandler;

class NetworkHelper
{
public:
    NetworkHelper(PacketHandler* packetHandler);
    virtual ~NetworkHelper();
    
    virtual void processSpecialPacket(uint8_t packetType, InputMemoryBitStream& inputStream, MachineAddress* fromAddress) = 0;
    virtual void processIncomingPackets();
    virtual void sendPacket(const OutputMemoryBitStream& outputStream, MachineAddress* fromAddress);
    
    const WeightedTimedMovingAverage& getBytesReceivedPerSecond() const;
    const WeightedTimedMovingAverage& getBytesSentPerSecond() const;
    
protected:
    PacketHandler* _packetHandler;
};
