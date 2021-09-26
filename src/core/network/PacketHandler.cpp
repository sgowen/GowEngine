//
//  PacketHandler.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

PacketHandler::PacketHandler(TimeTracker* tt, uint16_t port, ProcessPacketFunc ppf) :
_socket(nullptr),
_socketAddress(INADDR_ANY, port),
_timeTracker(tt),
_processPacketFunc(ppf),
_bytesSentThisFrame(0),
_bytesReceivedPerSecond(tt, 1.0f),
_bytesSentPerSecond(tt, 1.0f)
{
    // Empty
}

PacketHandler::~PacketHandler()
{
    if (_socket != nullptr)
    {
        delete _socket;
    }
}

int PacketHandler::connect()
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
    
    return _socket->setNonBlockingMode(true);
}

void PacketHandler::processIncomingPackets()
{
    readIncomingPacketsIntoQueue();
    processQueuedPackets();
    updateBytesSentLastFrame();
}

void PacketHandler::sendPacket(const OutputMemoryBitStream& ombs, SocketAddress* toAddress)
{
    int sentByteCount = _socket->sendToAddress(ombs.getBufferPtr(), ombs.getByteLength(), *toAddress);
    if (sentByteCount > 0)
    {
        _bytesSentThisFrame += sentByteCount;
    }
}

SocketAddress& PacketHandler::getSocketAddress()
{
    return _socketAddress;
}

void PacketHandler::readIncomingPacketsIntoQueue()
{
    static char packetMem[NW_MAX_PACKET_SIZE];
    memset(packetMem, '\0', NW_MAX_PACKET_SIZE);

    InputMemoryBitStream imbs(packetMem, NW_MAX_PACKET_SIZE);
    SocketAddress fromAddress;

    int receivedPacketCount = 0;
    int totalReadByteCount = 0;

    while (receivedPacketCount < NW_MAX_NUM_PACKETS_PER_FRAME)
    {
        int readByteCount = _socket->receiveFromAddress(packetMem, NW_MAX_PACKET_SIZE, fromAddress);
        assert(readByteCount <= NW_MAX_PACKET_SIZE);
        
        if (readByteCount <= 0)
        {
            break;
        }
        else if (readByteCount > 0)
        {
            imbs.resetToCapacity(readByteCount);
            ++receivedPacketCount;
            totalReadByteCount += readByteCount;
            
            _packetQueue.emplace(_timeTracker->_time, imbs, fromAddress);
        }
    }

    if (totalReadByteCount > 0)
    {
        updateBytesReceivedLastFrame(totalReadByteCount);
    }
}

void PacketHandler::processQueuedPackets()
{
    while (!_packetQueue.empty())
    {
        ReceivedPacket& nextPacket = _packetQueue.front();
        if (_timeTracker->_time >= nextPacket.getReceivedTime())
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

const MovingAverage& PacketHandler::getBytesReceivedPerSecond() const
{
    return _bytesReceivedPerSecond;
}

const MovingAverage& PacketHandler::getBytesSentPerSecond() const
{
    return _bytesSentPerSecond;
}

void PacketHandler::updateBytesSentLastFrame()
{
    if (_bytesSentThisFrame > 0)
    {
        _bytesSentPerSecond.updatePerSecond(static_cast<float>(_bytesSentThisFrame));
        
        _bytesSentThisFrame = 0;
    }
}

void PacketHandler::updateBytesReceivedLastFrame(int totalReadByteCount)
{
    _bytesReceivedPerSecond.updatePerSecond(static_cast<float>(totalReadByteCount));
}

PacketHandler::ReceivedPacket::ReceivedPacket(uint32_t receivedTime, InputMemoryBitStream& imbs, SocketAddress fromAddress) :
_receivedTime(receivedTime),
_packetBuffer(imbs),
_fromAddress(fromAddress)
{
    // Empty
}

SocketAddress& PacketHandler::ReceivedPacket::getFromAddress()
{
    return _fromAddress;
}

uint32_t PacketHandler::ReceivedPacket::getReceivedTime() const
{
    return _receivedTime;
}

InputMemoryBitStream& PacketHandler::ReceivedPacket::getPacketBuffer()
{
    return _packetBuffer;
}
