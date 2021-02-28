//
//  SocketAddressFactory.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>

class SocketAddress;

class SocketAddressFactory
{
public:
    static SocketAddress* createIPv4FromString(const std::string& value);
    
private:
    SocketAddressFactory();
    ~SocketAddressFactory();
    SocketAddressFactory(const SocketAddressFactory&);
    SocketAddressFactory& operator=(const SocketAddressFactory&);
};
