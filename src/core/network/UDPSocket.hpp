//
//  UDPSocket.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Network.hpp"

class SocketAddress;

class UDPSocket
{
    friend class SocketUtil;
    
public:
    ~UDPSocket();
    
    int bindSocket(const SocketAddress& toAddress);
    int sendToAddress(const void* toSend, int length, const SocketAddress& toAddress);
    int receiveFromAddress(void* toReceive, int maxLength, SocketAddress& fromAddress);
    int setNonBlockingMode(bool nonBlocking);
    
private:
    SOCKET _socket;
    
    UDPSocket(SOCKET socket);
};
