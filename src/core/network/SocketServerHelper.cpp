//
//  SocketServerHelper.cpp
//  noctisgames
//
//  Created by Stephen Gowen on 6/17/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

SocketServerHelper::SocketServerHelper(TimeTracker& tt, uint16_t inPort, ProcessPacketFunc inProcessPacketFunc, GetClientProxyFunc inGetClientProxyFunc, HandleClientDisconnectedFunc inHandleClientDisconnectedFunc) : ServerHelper(new SocketPacketHandler(tt, inPort, inProcessPacketFunc), inGetClientProxyFunc, inHandleClientDisconnectedFunc)
{
    // Empty
}

SocketServerHelper::~SocketServerHelper()
{
    for (uint8_t i = 0; i < MAX_NUM_PLAYERS; ++i)
    {
        ClientProxy* clientProxy = _getClientProxyFunc(i);
        if (clientProxy)
        {
            SocketAddress* userAddress = static_cast<SocketAddress*>(clientProxy->getMachineAddress());
            
            OutputMemoryBitStream packet(NW_MAX_PACKET_SIZE);
            packet.write(static_cast<uint8_t>(NWPT_SRVR_EXIT));
            
            sendPacket(packet, userAddress);
        }
    }
}

int SocketServerHelper::connect()
{
    return _packetHandler->connect();
}

void SocketServerHelper::processSpecialPacket(uint8_t packetType, InputMemoryBitStream& inInputStream, MachineAddress* inFromAddress)
{
    switch (packetType)
    {
        case NWPT_CLNT_EXIT:
        {
            LOG("Client is leaving the server");
            
            size_t socketAddrHashRemote = (static_cast<SocketAddress*>(inFromAddress))->getHash();
            
            // Find the connection that should exist for this users address
            bool isFound = false;
            for (uint8_t i = 0; i < MAX_NUM_PLAYERS; ++i)
            {
                ClientProxy* clientProxy = _getClientProxyFunc(i);
                if (clientProxy)
                {
                    SocketAddress* userAddress = static_cast<SocketAddress*>(clientProxy->getMachineAddress());
                    if (userAddress->getHash() == socketAddrHashRemote)
                    {
                        isFound = true;
                        _handleClientDisconnectedFunc(clientProxy);
                        break;
                    }
                }
            }
            
            if (!isFound)
            {
                LOG("Got a client leaving server msg, but couldn't find a matching client");
            }
        }
            break;
        default:
            // Socket based Networking doesn't have built-in auth, so there should never be special packets besides client exit
            LOG("Unknown packet type received from %s", inFromAddress->toString().c_str());
            break;
    }
}

void SocketServerHelper::onClientDisconnected(ClientProxy* clientProxy)
{
    UNUSED(clientProxy);
    
    // Nothing to do here.
}

MachineAddress* SocketServerHelper::getServerAddress()
{
    SocketAddress& sa = (static_cast<SocketPacketHandler*>(_packetHandler))->getSocketAddress();
    
    return &sa;
}

bool SocketServerHelper::isConnected()
{
    // TODO ?
    return true;
}
