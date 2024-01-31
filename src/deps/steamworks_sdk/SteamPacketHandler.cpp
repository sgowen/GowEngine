//
//  SteamPacketHandler.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_DESKTOP

SteamPacketHandler::SteamPacketHandler(TimeTracker& tt,
                                       bool isServer,
                                       ProcessPacketFunc ppf) : PacketHandler(tt, ppf),
_isServer(isServer)
{
    // Empty
}

SteamPacketHandler::~SteamPacketHandler()
{
    // Empty
}

int SteamPacketHandler::connect()
{
    // No need to manually create the UDP socket with Steam I guess?
    return NO_ERROR;
}

void SteamPacketHandler::sendPacket(const OutputMemoryBitStream& inOutputStream, MachineAddress* inFromAddress)
{
    SteamAddress* inFromSteamAddress = static_cast<SteamAddress*>(inFromAddress);
    
    ISteamNetworking* steamNetworking = _isServer ? SteamGameServerNetworking() : SteamNetworking();
    
    if (steamNetworking->SendP2PPacket(inFromSteamAddress->getSteamID(), inOutputStream.getBufferPtr(), inOutputStream.getByteLength(), inFromSteamAddress->isReliable() ? k_EP2PSendReliable : k_EP2PSendUnreliable))
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

void SteamPacketHandler::readIncomingPacketsIntoQueue()
{
    static char packetMem[NW_MAX_PACKET_SIZE];
    static uint32 packetSize = sizeof(packetMem);
    
    bzero(packetMem, NW_MAX_PACKET_SIZE);
    
    uint32_t incomingSize = 0;
    InputMemoryBitStream inputStream(packetMem, packetSize * 8);
    CSteamID fromId;
    
    int totalReadByteCount = 0;
    
    uint8_t numFramesOfSimulatedLatency = ENGINE_CFG.numFramesOfSimulatedLatency();
    
    ISteamNetworking* steamNetworking = _isServer ? SteamGameServerNetworking() : SteamNetworking();
    
    while (steamNetworking->IsP2PPacketAvailable(&incomingSize))
    {
        if (incomingSize <= packetSize)
        {
            uint32_t readByteCount;
            if (steamNetworking->ReadP2PPacket(packetMem, packetSize, &readByteCount, &fromId))
            {
                assert(readByteCount <= packetSize);
                
                if (readByteCount > 0)
                {
                    inputStream.resetToCapacity(readByteCount);
                    totalReadByteCount += readByteCount;
                    
                    _packetQueue.emplace(_timeTracker._time + numFramesOfSimulatedLatency, inputStream, fromId);
                }
            }
        }
    }
    
    if (totalReadByteCount > 0)
    {
        updateBytesReceivedLastFrame(totalReadByteCount);
    }
}

void SteamPacketHandler::processQueuedPackets()
{
    //look at the front packet...
    while (!_packetQueue.empty())
    {
        ReceivedPacket& nextPacket = _packetQueue.front();
        if (_timeTracker._time > nextPacket.getReceivedTime())
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

SteamPacketHandler::ReceivedPacket::ReceivedPacket(uint32_t inReceivedTime, InputMemoryBitStream& ioInputMemoryBitStream, SteamAddress inFromAddress) :
_receivedTime(inReceivedTime),
_fromAddress(inFromAddress),
_packetBuffer(ioInputMemoryBitStream)
{
    // Empty
}

SteamAddress& SteamPacketHandler::ReceivedPacket::getFromAddress()
{
    return _fromAddress;
}

uint32_t SteamPacketHandler::ReceivedPacket::getReceivedTime() const
{
    return _receivedTime;
}

InputMemoryBitStream& SteamPacketHandler::ReceivedPacket::getPacketBuffer()
{
    return _packetBuffer;
}

#endif /* IS_DESKTOP */
