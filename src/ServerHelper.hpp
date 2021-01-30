//
//  ServerHelper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/17/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "NetworkHelper.hpp"

class ClientProxy;

typedef ClientProxy* (*GetClientProxyFunc)(uint8_t playerIndex);
typedef void (*HandleClientDisconnectedFunc)(ClientProxy* clientProxy);

class ServerHelper : public NetworkHelper
{
public:
    ServerHelper(uint8_t maxNumPlayers, PacketHandler* packetHandler, GetClientProxyFunc getClientProxyFunc, HandleClientDisconnectedFunc handleClientDisconnectedFunc);
    virtual ~ServerHelper() {}
    
    virtual void onClientDisconnected(ClientProxy* clientProxy) = 0;
    virtual MachineAddress* getServerAddress() = 0;
    virtual bool isConnected() = 0;
    
    uint8_t maxNumPlayers();
    
protected:
    uint8_t _maxNumPlayers;
    GetClientProxyFunc _getClientProxyFunc;
    HandleClientDisconnectedFunc _handleClientDisconnectedFunc;
};
