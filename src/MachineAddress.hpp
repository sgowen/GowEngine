//
//  MachineAddress.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>

class MachineAddress
{
public:
    MachineAddress() {}
    virtual ~MachineAddress() {}
    
    virtual MachineAddress* createNewCopy() = 0;
    virtual uint64_t getHash() const = 0;
    virtual std::string toString() const = 0;
};
