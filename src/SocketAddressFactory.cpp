//
//  SocketAddressFactory.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "SocketAddressFactory.hpp"

#include "SocketAddress.hpp"

#include "SocketUtil.hpp"
#include "StringUtil.hpp"

#include <cstring>

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
        LOG("Using default port of 0, is that what you want?");
        service = "0";
    }
    
    addrinfo hint;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;
    
    addrinfo* result = NULL;
    int error = getaddrinfo(host.c_str(), service.c_str(), &hint, &result);
    if (error != 0 && result != NULL)
    {
        SOCKET_UTIL.reportError("SocketAddressFactory::createIPv4FromString");
        return NULL;
    }
    
    if (result == NULL)
    {
        SOCKET_UTIL.reportError("SocketAddressFactory::createIPv4FromString");
        return NULL;
    }
    
    while (!result->ai_addr && result->ai_next)
    {
        result = result->ai_next;
    }
    
    if (!result->ai_addr)
    {
        return NULL;
    }
    
    SocketAddress* ret = new SocketAddress(*result->ai_addr);
    
    freeaddrinfo(result);
    
    return ret;
}
