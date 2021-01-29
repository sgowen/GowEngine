//
//  SocketServerHelper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/17/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "SocketServerHelper.hpp"

#include "SocketAddress.hpp"
#include "SocketPacketHandler.hpp"
#include "Macros.hpp"
#include "StringUtil.hpp"
#include "ClientProxy.hpp"
#include "OutputMemoryBitStream.hpp"
#include "InstanceManager.hpp"

SocketServerHelper::SocketServerHelper(uint16_t port, ProcessPacketFunc processPacketFunc, HandleNoResponseFunc handleNoResponseFunc, HandleConnectionResetFunc handleConnectionResetFunc, GetClientProxyFunc getClientProxyFunc, HandleClientDisconnectedFunc handleClientDisconnectedFunc) : ServerHelper(new SocketPacketHandler(static_cast<Timing*>(INSTANCE_MGR.get(InstanceKey_TIMING_SERVER)), true, port, processPacketFunc, handleNoResponseFunc, handleConnectionResetFunc), getClientProxyFunc, handleClientDisconnectedFunc)
{
    // Empty
}

SocketServerHelper::~SocketServerHelper()
{
    for (uint8_t i = 0; i < NW_MAX_NUM_PLAYERS; ++i)
    {
        ClientProxy* clientProxy = _getClientProxyFunc(i);
        if (clientProxy)
        {
            SocketAddress* userAddress = static_cast<SocketAddress*>(clientProxy->getMachineAddress());
            
            OutputMemoryBitStream packet;
            packet.write(static_cast<uint8_t>(NetworkPacketType_SERVER_EXIT));
            
            sendPacket(packet, userAddress);
        }
    }
}

void SocketServerHelper::processSpecialPacket(uint8_t packetType, InputMemoryBitStream& inputStream, MachineAddress* fromAddress)
{
    switch (packetType)
    {
        case NetworkPacketType_CLIENT_EXIT:
        {
            LOG("Client is leaving the server");
            
            size_t socketAddrHashRemote = (static_cast<SocketAddress*>(fromAddress))->getHash();
            
            // Find the connection that should exist for this users address
            bool isFound = false;
            for (uint8_t i = 0; i < NW_MAX_NUM_PLAYERS; ++i)
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
            LOG("Unknown packet type received from %s", fromAddress->toString().c_str());
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
    return (static_cast<SocketPacketHandler*>(_packetHandler))->getSocketAddress();
}

bool SocketServerHelper::isConnected()
{
    return (static_cast<SocketPacketHandler*>(_packetHandler))->isInitialized();
}
