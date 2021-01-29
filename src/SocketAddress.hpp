//
//  SocketAddress.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "MachineAddress.hpp"

#include "Network.hpp"

#include <string>

class SocketAddress : public MachineAddress
{
    friend class UDPSocket;
    
public:
    SocketAddress(uint32_t address, uint16_t port);
    SocketAddress(const sockaddr& socketAddress);
    SocketAddress(sockaddr& socketAddress);
    SocketAddress();
    
    virtual MachineAddress* createNewCopy();
    virtual uint64_t getHash() const;
    virtual std::string toString() const;
    
    bool operator==(const SocketAddress& other) const;
    uint32_t getSize() const;

private:
    sockaddr _socketAddress;
    
    uint32_t& getIP4Ref();
    const uint32_t& getIP4Ref() const;
    sockaddr_in* getAsSockAddrIn();
    const sockaddr_in* getAsSockAddrIn() const;
    sockaddr_in6* getAsSockAddrIn6();
    const sockaddr_in6* getAsSockAddrIn6() const;
};
