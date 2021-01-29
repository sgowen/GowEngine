//
//  SocketUtil.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "SocketUtil.hpp"

#include "UDPSocket.hpp"

#include "Network.hpp"
#include "StringUtil.hpp"
#include "PlatformMacros.hpp"

SocketUtil& SocketUtil::getInstance()
{
    static SocketUtil ret = SocketUtil();
    return ret;
}

bool SocketUtil::init()
{
    if (_isInitialized)
    {
        return true;
    }
    
#if IS_WINDOWS
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
    {
        reportError("Starting Up");
        return false;
    }
#endif
    
    _isInitialized = true;
    
    return true;
}

void SocketUtil::reportError(const char* operationDesc)
{
#if IS_WINDOWS
    LPVOID lpMsgBuf;
    DWORD errorNum = getLastError();
    
    FormatMessage(
                  FORMAT_MESSAGE_ALLOCATE_BUFFER |
                  FORMAT_MESSAGE_FROM_SYSTEM |
                  FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL,
                  errorNum,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPTSTR) &lpMsgBuf,
                  0, NULL);
    
    
    LOG("Error %s: %d- %s", operationDesc, errorNum, lpMsgBuf);
#else
    LOG("Error: %hs", operationDesc);
#endif
}

int SocketUtil::getLastError()
{
#if IS_WINDOWS
    return WSAGetLastError();
#else
    return errno;
#endif
}

UDPSocket* SocketUtil::createUDPSocket(SocketAddressFamily saf)
{
    SOCKET s = socket(saf, SOCK_DGRAM, IPPROTO_UDP);
    
    if (s != INVALID_SOCKET)
    {
        return new UDPSocket(s);
    }
    else
    {
        reportError("SocketUtil::createUDPSocket");
        
        return NULL;
    }
}

SocketUtil::SocketUtil() :
_isInitialized(false)
{
    // Empty
}

SocketUtil::~SocketUtil()
{
    if (_isInitialized)
    {
#if IS_WINDOWS
        WSACleanup();
#endif
    }
}
