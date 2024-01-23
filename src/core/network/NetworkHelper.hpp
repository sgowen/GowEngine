//
//  NetworkHelper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/17/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <cstdint>

class InputMemoryBitStream;
class OutputMemoryBitStream;
class MachineAddress;
class MovingAverage;
class PacketHandler;

class NetworkHelper
{
public:
    NetworkHelper(PacketHandler* packetHandler);
    virtual ~NetworkHelper();
    
    virtual void processSpecialPacket(uint8_t packetType, InputMemoryBitStream& inInputStream, MachineAddress* inFromAddress) = 0;
    
    virtual void processIncomingPackets();
    virtual void sendPacket(const OutputMemoryBitStream& inOutputStream, MachineAddress* inFromAddress);
    
    const MovingAverage& bytesReceivedPerSecond() const;
    const MovingAverage& bytesSentPerSecond() const;
    
protected:
    PacketHandler* _packetHandler;
};
