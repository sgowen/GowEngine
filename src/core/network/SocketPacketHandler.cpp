//
//  SocketPacketHandler.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/23/24.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

SocketPacketHandler::SocketPacketHandler(TimeTracker& tt, uint16_t port, ProcessPacketFunc ppf) : PacketHandler(tt, ppf),
_socket(nullptr),
_socketAddress(INADDR_ANY, port)
{
    // Empty
}

SocketPacketHandler::~SocketPacketHandler()
{
    if (_socket != nullptr)
    {
        delete _socket;
    }
}

int SocketPacketHandler::connect()
{
    if (!SOCKET_UTIL.init())
    {
        return NO_ERROR;
    }
    
    _socket = SOCKET_UTIL.createUDPSocket(INET);
    int bindSocketRet = _socket->bindSocket(_socketAddress);
    if (bindSocketRet != NO_ERROR)
    {
        return bindSocketRet;
    }
    
    int result = _socket->setNonBlockingMode(true);
    
    return result;
}

void SocketPacketHandler::sendPacket(const OutputMemoryBitStream& ombs, MachineAddress* toAddress)
{
    SocketAddress* toSocketAddress = static_cast<SocketAddress*>(toAddress);
    int sentByteCount = _socket->sendToAddress(ombs.getBufferPtr(), ombs.getByteLength(), toSocketAddress);
    if (sentByteCount > 0)
    {
        _bytesSentThisFrame += sentByteCount;
    }
}

void SocketPacketHandler::processIncomingPackets()
{
    readIncomingPacketsIntoQueue();
    processQueuedPackets();
    updateBytesSentLastFrame();
}

SocketAddress& SocketPacketHandler::getSocketAddress()
{
    return _socketAddress;
}

void SocketPacketHandler::readIncomingPacketsIntoQueue()
{
    static char packetMem[1500];
    memset(packetMem, '\0', 1500);

    InputMemoryBitStream imbs(packetMem, ENGINE_CFG.mtuSize());
    SocketAddress fromAddress;

    int totalReadByteCount = 0;

    uint8_t numFramesOfSimulatedLatency = ENGINE_CFG.numFramesOfSimulatedLatency();
    while (true)
    {
        int readByteCount = _socket->receiveFromAddress(packetMem, ENGINE_CFG.mtuSize(), fromAddress);
        assert(readByteCount <= ENGINE_CFG.mtuSize());
        
        if (readByteCount <= 0)
        {
            break;
        }
        else if (readByteCount > 0)
        {
            imbs.resetToCapacity(readByteCount);
            totalReadByteCount += readByteCount;
            
            _packetQueue.emplace(_timeTracker._time + numFramesOfSimulatedLatency, imbs, fromAddress);
        }
    }

    if (totalReadByteCount > 0)
    {
        updateBytesReceivedLastFrame(totalReadByteCount);
    }
}

void SocketPacketHandler::processQueuedPackets()
{
    while (!_packetQueue.empty())
    {
        ReceivedPacket& nextPacket = _packetQueue.front();
        if (_timeTracker._time >= nextPacket.getReceivedTime())
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

SocketPacketHandler::ReceivedPacket::ReceivedPacket(uint32_t receivedTime, InputMemoryBitStream& imbs, SocketAddress fromAddress) :
_receivedTime(receivedTime),
_packetBuffer(imbs),
_fromAddress(fromAddress)
{
    // Empty
}

SocketAddress& SocketPacketHandler::ReceivedPacket::getFromAddress()
{
    return _fromAddress;
}

uint32_t SocketPacketHandler::ReceivedPacket::getReceivedTime() const
{
    return _receivedTime;
}

InputMemoryBitStream& SocketPacketHandler::ReceivedPacket::getPacketBuffer()
{
    return _packetBuffer;
}
