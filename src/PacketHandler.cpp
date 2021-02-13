//
//  PacketHandler.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "PacketHandler.hpp"

#include "TimeTracker.hpp"
#include "WeightedTimedMovingAverage.hpp"
#include "InputMemoryBitStream.hpp"
#include "OutputMemoryBitStream.hpp"
#include "SocketAddress.hpp"
#include "StringUtil.hpp"
#include "SocketUtil.hpp"
#include "UDPSocket.hpp"

PacketHandler::PacketHandler(TimeTracker* timing, bool isServer, uint16_t port, ProcessPacketFunc processPacketFunc, HandleNoResponseFunc handleNoResponseFunc, HandleConnectionResetFunc handleConnectionResetFunc) :
_timeTracker(timing),
_processPacketFunc(processPacketFunc),
_handleNoResponseFunc(handleNoResponseFunc),
_handleConnectionResetFunc(handleConnectionResetFunc),
_bytesReceivedPerSecond(timing, 1.0f),
_bytesSentPerSecond(timing, 1.0f),
_bytesSentThisFrame(0),
_isServer(isServer),
_socket(NULL),
_socketAddress(INADDR_ANY, port),
_isConnected(false)
{
    if (!SOCKET_UTIL.init())
    {
        return;
    }

    _socket = SOCKET_UTIL.createUDPSocket(INET);
    int bindSocketResult = _socket->bindSocket(_socketAddress);
    if (bindSocketResult != NO_ERROR)
    {
        LOG("bindSocket failed. Error code %d", bindSocketResult);
        return;
    }

    LOG("Initializing PacketHandler at port %hu", port);
    _isConnected = _socket->setNonBlockingMode(true) == NO_ERROR;
}

PacketHandler::~PacketHandler()
{
    if (_socket != NULL)
    {
        delete _socket;
    }
}

void PacketHandler::processIncomingPackets()
{
    readIncomingPacketsIntoQueue();
    
    processQueuedPackets();
    
    updateBytesSentLastFrame();
}

void PacketHandler::sendPacket(const OutputMemoryBitStream& ombs, SocketAddress* fromAddress)
{
    assert(_isConnected);
    
#if IS_DEBUG
    LOG("%s Outgoing packet Bit Length: %d", _isServer ? "Server" : "Client", ombs.getBitLength());
#endif

    int sentByteCount = _socket->sendToAddress(ombs.getBufferPtr(), ombs.getByteLength(), *fromAddress);
    if (sentByteCount > 0)
    {
        _bytesSentThisFrame += sentByteCount;
    }
}

bool PacketHandler::isConnected()
{
    return _isConnected;
}

SocketAddress& PacketHandler::getSocketAddress()
{
    return _socketAddress;
}

void PacketHandler::readIncomingPacketsIntoQueue()
{
    assert(_isConnected);

    static char packetMem[NW_MAX_PACKET_SIZE];
    bzero(packetMem, NW_MAX_PACKET_SIZE);

    InputMemoryBitStream imbs(packetMem, NW_MAX_PACKET_SIZE * 8);
    SocketAddress fromAddress;

    // keep reading until we don't have anything to read (or we hit a max number that we'll process per frame)
    int receivedPacketCount = 0;
    int totalReadByteCount = 0;

    while (receivedPacketCount < NW_MAX_NUM_PACKETS_PER_FRAME)
    {
        int readByteCount = _socket->receiveFromAddress(packetMem, NW_MAX_PACKET_SIZE, fromAddress);
        assert(readByteCount <= NW_MAX_PACKET_SIZE);
        
        if (readByteCount == 0)
        {
            // nothing to read
            _handleNoResponseFunc();
            break;
        }
        else if (readByteCount == -WSAECONNRESET)
        {
            // port closed on other end, so DC this person immediately
            _handleConnectionResetFunc(&fromAddress);
        }
        else if (readByteCount > 0)
        {
            imbs.resetToCapacity(readByteCount);
            ++receivedPacketCount;
            totalReadByteCount += readByteCount;
            
#if IS_DEBUG
            LOG("%s readByteCount: %d", _isServer ? "Server" : "Client", readByteCount);
#endif

            // we made it
            // shove the packet into the queue and we'll handle it as soon as we should...
            // we'll pretend it wasn't received until simulated latency from now
            // this doesn't sim jitter, for that we would need to.....

            float simulatedReceivedTime = _timeTracker->_time;
            _packetQueue.push(ReceivedPacket(simulatedReceivedTime, imbs, fromAddress));
        }
        else
        {
            // uhoh, error? exit or just keep going?
        }
    }

    if (totalReadByteCount > 0)
    {
        updateBytesReceivedLastFrame(totalReadByteCount);
    }
}

void PacketHandler::processQueuedPackets()
{
    // look at the front packet...
    while (!_packetQueue.empty())
    {
        ReceivedPacket& nextPacket = _packetQueue.front();
        if (_timeTracker->_time > nextPacket.getReceivedTime())
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

const WeightedTimedMovingAverage& PacketHandler::getBytesReceivedPerSecond() const
{
    return _bytesReceivedPerSecond;
}

const WeightedTimedMovingAverage& PacketHandler::getBytesSentPerSecond() const
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

PacketHandler::ReceivedPacket::ReceivedPacket(float receivedTime, InputMemoryBitStream& imbs, SocketAddress fromAddress) :
_receivedTime(receivedTime),
_fromAddress(fromAddress),
_packetBuffer(imbs)
{
    // Empty
}

SocketAddress& PacketHandler::ReceivedPacket::getFromAddress()
{
    return _fromAddress;
}

float PacketHandler::ReceivedPacket::getReceivedTime() const
{
    return _receivedTime;
}

InputMemoryBitStream& PacketHandler::ReceivedPacket::getPacketBuffer()
{
    return _packetBuffer;
}
