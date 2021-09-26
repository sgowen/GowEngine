//
//  SocketAddressFactory.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

SocketAddress* SocketAddressFactory::createIPv4FromString(const std::string& value)
{
    auto pos = value.find_last_of(':');
    std::string host, service;
    
    if (pos != std::string::npos)
    {
        host = value.substr(0, pos);
        service = value.substr(pos + 1);
    }
    else
    {
        host = value;
        service = "0";
    }
    
    addrinfo hint;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;
    
    addrinfo* result = nullptr;
    int error = getaddrinfo(host.c_str(), service.c_str(), &hint, &result);
    if (error != 0 && result != nullptr)
    {
        SOCKET_UTIL.reportError("SocketAddressFactory::createIPv4FromString");
        return nullptr;
    }
    
    if (result == nullptr)
    {
        SOCKET_UTIL.reportError("SocketAddressFactory::createIPv4FromString");
        return nullptr;
    }
    
    while (!result->ai_addr && result->ai_next)
    {
        result = result->ai_next;
    }
    
    if (!result->ai_addr)
    {
        return nullptr;
    }
    
    SocketAddress* ret = new SocketAddress(*result->ai_addr);
    
    freeaddrinfo(result);
    
    return ret;
}
