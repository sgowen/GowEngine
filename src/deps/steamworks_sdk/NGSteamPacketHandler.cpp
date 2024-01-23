//
//  NGSteamPacketHandler.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_DESKTOP

NGSteamPacketHandler::NGSteamPacketHandler(TimeTracker& tt,
                                           ISteamNetworking* steamNetworking,
                                           ProcessPacketFunc ppf) : PacketHandler(tt, ppf),
_steamNetworking(steamNetworking)
{
    // Empty
}

NGSteamPacketHandler::~NGSteamPacketHandler()
{
    // Empty
}

void NGSteamPacketHandler::sendPacket(const OutputMemoryBitStream& inOutputStream, MachineAddress* inFromAddress)
{
#ifdef NG_LOG
    LOG("%s Outgoing packet Bit Length: %d \n", _isServer ? "Server" : "Client", inOutputStream.getBitLength());
#endif
    
    NGSteamAddress* inFromSteamAddress = static_cast<NGSteamAddress*>(inFromAddress);
    
    if (_steamNetworking->SendP2PPacket(inFromSteamAddress->getSteamID(), inOutputStream.getBufferPtr(), inOutputStream.getByteLength(), inFromSteamAddress->isReliable() ? k_EP2PSendReliable : k_EP2PSendUnreliable))
    {
        int sentByteCount = inOutputStream.getByteLength();
        if (sentByteCount > 0)
        {
            _bytesSentThisFrame += sentByteCount;
        }
    }
    else
    {
        LOG("Failed sending data to server");
    }
}

void NGSteamPacketHandler::readIncomingPacketsIntoQueue()
{
    static char packetMem[NW_MAX_PACKET_SIZE];
    static uint32 packetSize = sizeof(packetMem);
    
    bzero(packetMem, NW_MAX_PACKET_SIZE);
    
    uint32_t incomingSize = 0;
    InputMemoryBitStream inputStream(packetMem, packetSize * 8);
    CSteamID fromId;
    
    int totalReadByteCount = 0;
    
    while (_steamNetworking->IsP2PPacketAvailable(&incomingSize))
    {
        if (incomingSize <= packetSize)
        {
            uint32_t readByteCount;
            if (_steamNetworking->ReadP2PPacket(packetMem, packetSize, &readByteCount, &fromId))
            {
                assert(readByteCount <= packetSize);
                
                if (readByteCount > 0)
                {
                    inputStream.resetToCapacity(readByteCount);
                    totalReadByteCount += readByteCount;
                    
                    //shove the packet into the queue and we'll handle it as soon as we should...
                    //we'll pretend it wasn't received until simulated latency from now
                    //this doesn't sim jitter, for that we would need to.....
                    float simulatedReceivedTime = _timeTracker.realTime();
                    
                    _packetQueue.push(ReceivedPacket(simulatedReceivedTime, inputStream, fromId));
                }
                
#ifdef NG_LOG
                if (!_isServer)
                {
                    LOG("readByteCount: %d", readByteCount);
                }
#endif
            }
        }
    }
    
    if (totalReadByteCount > 0)
    {
        updateBytesReceivedLastFrame(totalReadByteCount);
    }
}

void NGSteamPacketHandler::processQueuedPackets()
{
    //look at the front packet...
    while (!_packetQueue.empty())
    {
        ReceivedPacket& nextPacket = _packetQueue.front();
        if (_timeTracker.realTime() > nextPacket.getReceivedTime())
        {
            _processPacketFunc(nextPacket.getPacketBuffer(), &nextPacket.getFromAddress());
            _packetQueue.pop();
        }
        else
        {
            break;
        }
    }
}

NGSteamPacketHandler::ReceivedPacket::ReceivedPacket(float inReceivedTime, InputMemoryBitStream& ioInputMemoryBitStream, NGSteamAddress inFromAddress) :
_receivedTime(inReceivedTime),
_fromAddress(inFromAddress),
_packetBuffer(ioInputMemoryBitStream)
{
    // Empty
}

NGSteamAddress& NGSteamPacketHandler::ReceivedPacket::getFromAddress()
{
    return _fromAddress;
}

float NGSteamPacketHandler::ReceivedPacket::getReceivedTime() const
{
    return _receivedTime;
}

InputMemoryBitStream& NGSteamPacketHandler::ReceivedPacket::getPacketBuffer()
{
    return _packetBuffer;
}

#endif /* IS_DESKTOP */
