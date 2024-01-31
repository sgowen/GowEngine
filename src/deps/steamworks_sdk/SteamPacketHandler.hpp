//
//  SteamPacketHandler.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildConstants.hpp>
#if IS_DESKTOP

#include "core/network/PacketHandler.hpp"

#include "core/network/InputMemoryBitStream.hpp"
#include "deps/steamworks_sdk/SteamAddress.hpp"

#include <queue>
#include <list>

class OutputMemoryBitStream;
class MachineAddress;

class SteamPacketHandler : public PacketHandler
{
public:
    SteamPacketHandler(TimeTracker& timeTracker,
                       bool isServer,
                       ProcessPacketFunc processPacketFunc);
    virtual ~SteamPacketHandler();
    
    virtual int connect() override;
    virtual void sendPacket(const OutputMemoryBitStream& inOutputStream, MachineAddress* inFromAddress) override;

protected:
    virtual void readIncomingPacketsIntoQueue() override;
    virtual void processQueuedPackets() override;
    
private:
    bool _isServer;
    
    class ReceivedPacket;
    std::queue<ReceivedPacket, std::list<ReceivedPacket> > _packetQueue;
    
    class ReceivedPacket
    {
    public:
        ReceivedPacket(uint32_t inReceivedTime, InputMemoryBitStream& inInputMemoryBitStream, SteamAddress inFromAddress);
        
        SteamAddress& getFromAddress();
        uint32_t getReceivedTime()	const;
        InputMemoryBitStream& getPacketBuffer();
        
    private:
        uint32_t _receivedTime;
        InputMemoryBitStream _packetBuffer;
        SteamAddress _fromAddress;
    };
};

#endif /* IS_DESKTOP */
