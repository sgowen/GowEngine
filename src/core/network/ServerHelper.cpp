//
//  ServerHelper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/17/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

ServerHelper::ServerHelper(PacketHandler* packetHandler, GetClientProxyFunc inGetClientProxyFunc, HandleClientDisconnectedFunc inHandleClientDisconnectedFunc) : NetworkHelper(packetHandler),
_getClientProxyFunc(inGetClientProxyFunc),
_handleClientDisconnectedFunc(inHandleClientDisconnectedFunc)
{
    // Empty
}

ServerHelper::~ServerHelper()
{
    // Empty
}
