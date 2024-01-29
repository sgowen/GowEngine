//
//  SocketClientHelper.cpp
//  noctisgames
//
//  Created by Stephen Gowen on 6/17/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

SocketClientHelper::SocketClientHelper(TimeTracker& tt, std::string serverIPAddress, std::string inName, uint16_t inPort, ProcessPacketFunc processPacketFunc) :
ClientHelper(new SocketPacketHandler(tt, inPort, processPacketFunc)),
_serverAddress(SocketAddressFactory::createIPv4FromString(serverIPAddress)),
_name(inName)
{
    if (_serverAddress == nullptr &&
        ENGINE_CFG.networkLoggingEnabled())
    {
        LOG("Server Address invalid: %s", serverIPAddress.c_str());
    }
}

SocketClientHelper::~SocketClientHelper()
{
    OutputMemoryBitStream packet(NW_MAX_PACKET_SIZE);
    packet.write(static_cast<uint8_t>(NWPT_CLNT_EXIT));
    sendPacket(packet);
    
    if (_serverAddress)
    {
        delete _serverAddress;
    }
}

int SocketClientHelper::connect()
{
    return _packetHandler->connect();
}

void SocketClientHelper::processSpecialPacket(uint8_t packetType, InputMemoryBitStream& inInputStream, MachineAddress* inFromAddress)
{
    switch (packetType)
    {
        case NWPT_SRVR_EXIT:
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

void SocketClientHelper::sendPacket(const OutputMemoryBitStream& inOutputStream)
{
    if (_serverAddress)
    {
        NetworkHelper::sendPacket(inOutputStream, _serverAddress);
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
