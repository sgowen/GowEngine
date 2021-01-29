//
//  ServerHelper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/17/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "ServerHelper.hpp"

ServerHelper::ServerHelper(PacketHandler* packetHandler, GetClientProxyFunc getClientProxyFunc, HandleClientDisconnectedFunc handleClientDisconnectedFunc) : NetworkHelper(packetHandler),
_getClientProxyFunc(getClientProxyFunc),
_handleClientDisconnectedFunc(handleClientDisconnectedFunc)
{
    // Empty
}
