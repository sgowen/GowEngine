//
//  SocketUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Network.hpp"

class UDPSocket;

class SocketUtil
{
public:
    static SocketUtil& getInstance();
    
    bool init();
    void reportError(const char* operationDesc);
    int getLastError();
    UDPSocket* createUDPSocket(SocketAddressFamily saf);
    
private:
    bool _isConnected;
    
    SocketUtil();
    ~SocketUtil();
    SocketUtil(const SocketUtil&);
    SocketUtil& operator=(const SocketUtil&);
};
