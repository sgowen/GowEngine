//
//  SocketClientHelper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/17/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "SocketClientHelper.hpp"

#include "SocketAddress.hpp"

#include "SocketPacketHandler.hpp"
#include "SocketAddressFactory.hpp"
#include "OutputMemoryBitStream.hpp"
#include "StringUtil.hpp"
#include "InstanceManager.hpp"

SocketClientHelper::SocketClientHelper(std::string serverIPAddress, std::string name, uint16_t port, ProcessPacketFunc processPacketFunc, HandleNoResponseFunc handleNoResponseFunc, HandleConnectionResetFunc handleConnectionResetFunc) :
ClientHelper(new SocketPacketHandler(static_cast<Timing*>(INSTANCE_MGR.get(InstanceKey_TIMING_CLIENT)), false, port, processPacketFunc, handleNoResponseFunc, handleConnectionResetFunc)),
_serverAddress(SocketAddressFactory::createIPv4FromString(serverIPAddress)),
_name(name)
{
    // Empty
}

SocketClientHelper::~SocketClientHelper()
{
    OutputMemoryBitStream packet;
    packet.write(static_cast<uint8_t>(NetworkPacketType_CLIENT_EXIT));
    sendPacket(packet);
    
    if (_serverAddress)
    {
        delete _serverAddress;
    }
}

void SocketClientHelper::processSpecialPacket(uint8_t packetType, InputMemoryBitStream& inputStream, MachineAddress* fromAddress)
{
    switch (packetType)
    {
        case NetworkPacketType_SERVER_EXIT:
            LOG("Server Shutting Down");
            
            if (_serverAddress)
            {
                delete _serverAddress;
                _serverAddress = NULL;
            }
            
            updateState();
        default:
            // Socket based Networking doesn't have built-in auth, so there should never be special packets besides server exit
            break;
    }
}

void SocketClientHelper::handleUninitialized()
{
    updateState();
}

void SocketClientHelper::sendPacket(const OutputMemoryBitStream& outputStream)
{
    if (_serverAddress != NULL)
    {
        NetworkHelper::sendPacket(outputStream, _serverAddress);
    }
}

std::string& SocketClientHelper::getName()
{
    return _name;
}

void SocketClientHelper::updateState()
{
    // Socket based Networking doesn't have built-in auth, so we should be ready to say Hello to the server immediately
    _state = _serverAddress ? CLIENT_READY_TO_SAY_HELLO : CLIENT_AUTH_FAILED;
}
