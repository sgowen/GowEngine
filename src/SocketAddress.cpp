//
//  SocketAddress.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "SocketAddress.hpp"

#include "StringUtil.hpp"
#include "PlatformMacros.hpp"

#include <cstring>

SocketAddress::SocketAddress(uint32_t address, uint16_t port)
{
    getAsSockAddrIn()->sin_family = AF_INET;
    getIP4Ref() = htonl(address);
    getAsSockAddrIn()->sin_port = htons(port);
}

SocketAddress::SocketAddress(const sockaddr& socketAddress)
{
    memcpy(&_socketAddress, &socketAddress, sizeof(sockaddr));
}

SocketAddress::SocketAddress(sockaddr& socketAddress)
{
    memcpy(&_socketAddress, &socketAddress, sizeof(sockaddr));
}

SocketAddress::SocketAddress()
{
    getAsSockAddrIn()->sin_family = AF_INET;
    getIP4Ref() = INADDR_ANY;
    getAsSockAddrIn()->sin_port = 0;
}

SocketAddress* SocketAddress::createNewCopy()
{
    return new SocketAddress(_socketAddress);
}

uint64_t SocketAddress::getHash() const
{
    return (getIP4Ref()) | ((static_cast<uint32_t>(getAsSockAddrIn()->sin_port)) <<13) | _socketAddress.sa_family;
}

std::string SocketAddress::toString() const
{
    static char buffer[256];
#if IS_WINDOWS
    USHORT port;
#else
    in_port_t port;
#endif
    
    switch (_socketAddress.sa_family)
    {
        case AF_INET:
        {
            const sockaddr_in *addr_in = getAsSockAddrIn();
    #if IS_WINDOWS
            void *addr = (void*) &(addr_in->sin_addr);
            inet_ntop(AF_INET, addr, buffer, INET_ADDRSTRLEN);
    #else
            inet_ntop(AF_INET, &(addr_in->sin_addr), buffer, INET_ADDRSTRLEN);
    #endif
            port = addr_in->sin_port;
        }
            break;
        case AF_INET6:
        {
            const sockaddr_in6 *addr_in6 = getAsSockAddrIn6();
    #if IS_WINDOWS
            void *addr = (void*) &(addr_in6->sin6_addr);
            inet_ntop(AF_INET6, addr, buffer, INET6_ADDRSTRLEN);
    #else
            inet_ntop(AF_INET6, &(addr_in6->sin6_addr), buffer, INET6_ADDRSTRLEN);
    #endif
            port = addr_in6->sin6_port;
        }
            break;
        default:
        {
            port = 0;
        }
            break;
    }

    return StringUtil::format("Socket Address %s:%d", buffer, ntohs(port));
}

bool SocketAddress::operator==(const SocketAddress& other) const
{
    return (_socketAddress.sa_family == AF_INET && getAsSockAddrIn()->sin_port == other.getAsSockAddrIn()->sin_port) && (getIP4Ref() == other.getIP4Ref());
}

uint32_t SocketAddress::getSize() const
{
    return sizeof(sockaddr);
}

#if IS_WINDOWS
uint32_t& SocketAddress::getIP4Ref()
{
    return *reinterpret_cast<uint32_t*>(&getAsSockAddrIn()->sin_addr.S_un.S_addr);
}

const uint32_t& SocketAddress::getIP4Ref() const
{
    return *reinterpret_cast<const uint32_t*>(&getAsSockAddrIn()->sin_addr.S_un.S_addr);
}
#else
uint32_t& SocketAddress::getIP4Ref()
{
    return getAsSockAddrIn()->sin_addr.s_addr;
}

const uint32_t& SocketAddress::getIP4Ref() const
{
    return getAsSockAddrIn()->sin_addr.s_addr;
}
#endif

sockaddr_in* SocketAddress::getAsSockAddrIn()
{
    return reinterpret_cast<sockaddr_in*>(&_socketAddress);
}

const sockaddr_in* SocketAddress::getAsSockAddrIn() const
{
    return reinterpret_cast<const sockaddr_in*>(&_socketAddress);
}

sockaddr_in6* SocketAddress::getAsSockAddrIn6()
{
    return reinterpret_cast<sockaddr_in6*>(&_socketAddress);
}

const sockaddr_in6* SocketAddress::getAsSockAddrIn6() const
{
    return reinterpret_cast<const sockaddr_in6*>(&_socketAddress);
}
