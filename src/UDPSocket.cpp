//
//  UDPSocket.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "UDPSocket.hpp"

#include "SocketAddress.hpp"

#include "SocketUtil.hpp"
#include "StringUtil.hpp"
#include "PlatformMacros.hpp"

int UDPSocket::bindSocket(const SocketAddress& bindAddress)
{
    int error = bind(_socket, &bindAddress._socketAddress, bindAddress.getSize());
    if (error != 0)
    {
        SOCKET_UTIL.reportError("UDPSocket::Bind");
        
        return SOCKET_UTIL.getLastError();
    }
    
    return NO_ERROR;
}

int UDPSocket::sendToAddress(const void* toSend, int length, const SocketAddress& toAddress)
{
    long byteSentCount = sendto(_socket,
                                static_cast<const char*>(toSend),
                                length,
                                0, &toAddress._socketAddress, toAddress.getSize());
    
    if (byteSentCount <= 0)
    {
        //we'll return error as negative number to indicate less than requested amount of bytes sent...
        SOCKET_UTIL.reportError("UDPSocket::SendTo");
        
        LOG("Error UDPSocket::SendTo %s", toAddress.toString().c_str());
        
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
            //this can happen if a client closed and we haven't DC'd yet.
            //this is the ICMP message being sent back saying the port on that computer is closed
            LOG("Connection reset from %s", fromAddress.toString().c_str());
            
            return -WSAECONNRESET;
        }
        else
        {
            SOCKET_UTIL.reportError("UDPSocket::ReceiveFrom");
            
            return -error;
        }
    }
}

int UDPSocket::setNonBlockingMode(bool shouldBeNonBlocking)
{
#if IS_WINDOWS
    u_long arg = shouldBeNonBlocking ? 1 : 0;
    int result = ioctlsocket(_socket, FIONBIO, &arg);
#else
    int flags = fcntl(_socket, F_GETFL, 0);
    flags = shouldBeNonBlocking ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
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
