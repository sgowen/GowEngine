//
//  ServerHelper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/17/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#ifndef __GowEngine__ServerHelper__
#define __GowEngine__ServerHelper__

#include "core/network/NetworkHelper.hpp"

class ClientProxy;

typedef ClientProxy* (*GetClientProxyFunc)(uint8_t inPlayerIndex);
typedef void (*HandleClientDisconnectedFunc)(ClientProxy* inClientProxy);

class ServerHelper : public NetworkHelper
{
public:
    ServerHelper(PacketHandler* packetHandler, GetClientProxyFunc inGetClientProxyFunc, HandleClientDisconnectedFunc inHandleClientDisconnectedFunc);
    
    virtual ~ServerHelper();
    
    virtual void onClientDisconnected(ClientProxy* clientProxy) = 0;
    
    virtual MachineAddress* getServerAddress() = 0;
    
    virtual bool isConnected() = 0;
    
protected:
    GetClientProxyFunc _getClientProxyFunc;
    HandleClientDisconnectedFunc _handleClientDisconnectedFunc;
};

#endif /* defined(__GowEngine__ServerHelper__) */
