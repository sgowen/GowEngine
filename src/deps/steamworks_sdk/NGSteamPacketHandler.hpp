//
//  NGSteamPacketHandler.hpp
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
#include "deps/steamworks_sdk/NGSteamAddress.hpp"

#include <queue>
#include <list>

class OutputMemoryBitStream;
class MachineAddress;

class NGSteamPacketHandler : public PacketHandler
{
public:
    NGSteamPacketHandler(TimeTracker& timeTracker,
                         ISteamNetworking* steamNetworking,
                         ProcessPacketFunc processPacketFunc);
    virtual ~NGSteamPacketHandler();
    
    virtual void sendPacket(const OutputMemoryBitStream& inOutputStream, MachineAddress* inFromAddress);

protected:
    virtual void readIncomingPacketsIntoQueue();
    virtual void processQueuedPackets();
    
private:
    ISteamNetworking* _steamNetworking;
    
    class ReceivedPacket;
    std::queue<ReceivedPacket, std::list<ReceivedPacket> > _packetQueue;
    
    class ReceivedPacket
    {
    public:
        ReceivedPacket(float inReceivedTime, InputMemoryBitStream& inInputMemoryBitStream, NGSteamAddress inFromAddress);
        
        NGSteamAddress& getFromAddress();
        float getReceivedTime()	const;
        InputMemoryBitStream& getPacketBuffer();
        
    private:
        float _receivedTime;
        InputMemoryBitStream _packetBuffer;
        NGSteamAddress _fromAddress;
    };
};

#endif /* IS_DESKTOP */
