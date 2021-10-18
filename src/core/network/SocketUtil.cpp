//
//  SocketUtil.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

SocketUtil& SocketUtil::getInstance()
{
    static SocketUtil ret = SocketUtil();
    return ret;
}

bool SocketUtil::init()
{
    if (_isConnected)
    {
        return true;
    }
    
#if IS_WINDOWS
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult == NO_ERROR)
    {
        _isConnected = true;
    }
    else
    {
        reportError("Starting Up");
    }
#else
    _isConnected = true;
#endif
    
    return _isConnected;
}

void SocketUtil::reportError(const char* operationDesc)
{
#if IS_WINDOWS
    LPVOID lpMsgBuf;
    DWORD errorNum = getLastError();
    
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                  FORMAT_MESSAGE_FROM_SYSTEM |
                  FORMAT_MESSAGE_IGNORE_INSERTS,
                  nullptr,
                  errorNum,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPTSTR) &lpMsgBuf,
                  0, nullptr);
    
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
        
        return nullptr;
    }
}

SocketUtil::SocketUtil() :
_isConnected(false)
{
    // Empty
}

SocketUtil::~SocketUtil()
{
    if (_isConnected)
    {
#if IS_WINDOWS
        WSACleanup();
#endif
    }
}
