//
//  SocketUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Network.hpp"

#define SOCKET_UTIL (SocketUtil::getInstance())
#define IS_NETWORK_LOGGING_ENABLED SOCKET_UTIL.isLoggingEnabled

class UDPSocket;

class SocketUtil
{
public:
    static SocketUtil& getInstance();
    
    bool init();
    void reportError(const char* operationDesc);
    int getLastError();
    UDPSocket* createUDPSocket(SocketAddressFamily saf);
    void setLoggingEnabled(bool isLoggingEnabled);
    bool isLoggingEnabled();
    
private:
    bool _isConnected;
    bool _isLoggingEnabled;
    
    SocketUtil();
    ~SocketUtil();
    SocketUtil(const SocketUtil&);
    SocketUtil& operator=(const SocketUtil&);
};
