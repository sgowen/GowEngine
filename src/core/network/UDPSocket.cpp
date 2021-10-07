//
//  UDPSocket.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

int UDPSocket::bindSocket(const SocketAddress& bindAddress)
{
    int error = bind(_socket, &bindAddress._socketAddress, bindAddress.getSize());
    if (error != NO_ERROR)
    {
        SOCKET_UTIL.reportError("UDPSocket::bindSocket");
        
        return SOCKET_UTIL.getLastError();
    }
    
    return error;
}

int UDPSocket::sendToAddress(const void* toSend, int length, const SocketAddress* toAddress)
{
    if (toAddress == nullptr)
    {
        LOG("toAddress is nullptr");
        return -SOCKET_UTIL.getLastError();
    }
    
    long byteSentCount = sendto(_socket,
                                static_cast<const char*>(toSend),
                                length,
                                0, &toAddress->_socketAddress, toAddress->getSize());
    
    if (byteSentCount <= 0)
    {
        SOCKET_UTIL.reportError("UDPSocket::sendToAddress");
        
        if (IS_NETWORK_LOGGING_ENABLED())
        {
            LOG("Error UDPSocket::sendToAddress %s", toAddress->toString().c_str());
        }
        
        return -SOCKET_UTIL.getLastError();
    }
    else
    {
        return static_cast<int>(byteSentCount);
    }
}

int UDPSocket::receiveFromAddress(void* toReceive, int maxLength, SocketAddress& fromAddress)
{
    socklen_t fromLength = fromAddress.getSize();
    
    long readByteCount = recvfrom(_socket,
                                  static_cast<char*>(toReceive),
                                  maxLength,
                                  0, &fromAddress._socketAddress, &fromLength);
    
    if (readByteCount >= 0)
    {
        return static_cast<int>(readByteCount);
    }
    else
    {
        int error = SOCKET_UTIL.getLastError();
        
        if (error == WSAEWOULDBLOCK)
        {
            return 0;
        }
        else if (error == WSAECONNRESET)
        {
            if (IS_NETWORK_LOGGING_ENABLED())
            {
                LOG("Connection reset from %s", fromAddress.toString().c_str());
            }
            
            return -WSAECONNRESET;
        }
        else
        {
            SOCKET_UTIL.reportError("UDPSocket::receiveFromAddress");
            
            return -error;
        }
    }
}

int UDPSocket::setNonBlockingMode(bool nonBlocking)
{
#if IS_WINDOWS
    u_long arg = nonBlocking ? 1 : 0;
    int result = ioctlsocket(_socket, FIONBIO, &arg);
#else
    int flags = fcntl(_socket, F_GETFL, 0);
    flags = nonBlocking ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
    int result = fcntl(_socket, F_SETFL, flags);
#endif
    
    if (result == SOCKET_ERROR)
    {
        SOCKET_UTIL.reportError("UDPSocket::setNonBlockingMode");
        
        return SOCKET_UTIL.getLastError();
    }
    else
    {
        return NO_ERROR;
    }
}

UDPSocket::UDPSocket(SOCKET socket) :
_socket(socket)
{
    // Empty
}

UDPSocket::~UDPSocket()
{
#if IS_WINDOWS
    closesocket(_socket);
#else
    close(_socket);
#endif
}
