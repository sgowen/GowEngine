//
//  SocketPacketHandler.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "SocketPacketHandler.hpp"

#include "TimeTracker.hpp"
#include "OutputMemoryBitStream.hpp"
#include "UDPSocket.hpp"
#include "MachineAddress.hpp"

#include "SocketAddress.hpp"
#include "InputMemoryBitStream.hpp"
#include "SocketUtil.hpp"
#include "StringUtil.hpp"
#include "PlatformMacros.hpp"
#include "Network.hpp"
#include "InstanceManager.hpp"

#include <string.h>
#include <assert.h>

SocketPacketHandler::SocketPacketHandler(TimeTracker* timing, bool isServer, uint16_t port, ProcessPacketFunc processPacketFunc, HandleNoResponseFunc handleNoResponseFunc, HandleConnectionResetFunc handleConnectionResetFunc) : PacketHandler(timing, isServer, processPacketFunc, handleNoResponseFunc, handleConnectionResetFunc),
_socketAddress(new SocketAddress(INADDR_ANY, port)),
_socket(NULL),
_isInitialized(false)
{
    if (!SOCKET_UTIL.init())
    {
        return;
    }

    _socket = SOCKET_UTIL.createUDPSocket(INET);
    int bindSocketResult = _socket->bindSocket(*_socketAddress);
    if (bindSocketResult != NO_ERROR)
    {
        LOG("bindSocket failed. Error code %d", bindSocketResult);
        assert(false);
    }

    LOG("Initializing SocketPacketHandler at port %hu", port);

    if (_socket != NULL &&
        _socket->setNonBlockingMode(true) == NO_ERROR)
    {
        _isInitialized = true;
    }
}

SocketPacketHandler::~SocketPacketHandler()
{
    delete _socketAddress;

    if (_socket)
    {
        delete _socket;
    }
}

void SocketPacketHandler::sendPacket(const OutputMemoryBitStream& outputStream, MachineAddress* fromAddress)
{
    if (!_isInitialized)
    {
        return;
    }
    
#if IS_DEBUG
    LOG("%s Outgoing packet Bit Length: %d \n", _isServer ? "Server" : "Client", outputStream.getBitLength());
#endif

    SocketAddress* fromSocketAddress = static_cast<SocketAddress*>(fromAddress);
    int sentByteCount = _socket->sendToAddress(outputStream.getBufferPtr(), outputStream.getByteLength(), *fromSocketAddress);
    if (sentByteCount > 0)
    {
        _bytesSentThisFrame += sentByteCount;
    }
}

SocketAddress* SocketPacketHandler::getSocketAddress()
{
    return _socketAddress;
}

bool SocketPacketHandler::isInitialized()
{
    return _isInitialized;
}

void SocketPacketHandler::readIncomingPacketsIntoQueue()
{
    if (!_isInitialized)
    {
        return;
    }

    static char packetMem[NW_MAX_PACKET_SIZE];
    bzero(packetMem, NW_MAX_PACKET_SIZE);

    InputMemoryBitStream inputStream(packetMem, NW_MAX_PACKET_SIZE * 8);
    SocketAddress fromAddress;

    //keep reading until we don't have anything to read (or we hit a max number that we'll process per frame)
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
            //port closed on other end, so DC this person immediately
            _handleConnectionResetFunc(&fromAddress);
        }
        else if (readByteCount > 0)
        {
            inputStream.resetToCapacity(readByteCount);
            ++receivedPacketCount;
            totalReadByteCount += readByteCount;
            
#if IS_DEBUG
            if (!_isServer)
            {
                LOG("readByteCount: %d", readByteCount);
            }
#endif

            //we made it
            //shove the packet into the queue and we'll handle it as soon as we should...
            //we'll pretend it wasn't received until simulated latency from now
            //this doesn't sim jitter, for that we would need to.....

            float simulatedReceivedTime = _timeTracker->_time;
            _packetQueue.push(ReceivedPacket(simulatedReceivedTime, inputStream, fromAddress));
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

void SocketPacketHandler::processQueuedPackets()
{
    //look at the front packet...
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

SocketPacketHandler::ReceivedPacket::ReceivedPacket(float receivedTime, InputMemoryBitStream& ioInputMemoryBitStream, SocketAddress fromAddress) :
_receivedTime(receivedTime),
_fromAddress(fromAddress),
_packetBuffer(ioInputMemoryBitStream)
{
    // Empty
}

SocketAddress& SocketPacketHandler::ReceivedPacket::getFromAddress()
{
    return _fromAddress;
}

float SocketPacketHandler::ReceivedPacket::getReceivedTime() const
{
    return _receivedTime;
}

InputMemoryBitStream& SocketPacketHandler::ReceivedPacket::getPacketBuffer()
{
    return _packetBuffer;
}
