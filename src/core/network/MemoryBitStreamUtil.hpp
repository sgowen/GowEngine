//
//  MemoryBitStreamUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/17/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

class InputMemoryBitStream;
class OutputMemoryBitStream;

class MemoryBitStreamUtil
{
public:
    static void read(InputMemoryBitStream& imbs, float& x, float& y);
    static void write(OutputMemoryBitStream& ombs, float& x, float& y);
    
private:
    MemoryBitStreamUtil();
    ~MemoryBitStreamUtil();
    MemoryBitStreamUtil(const MemoryBitStreamUtil&);
    MemoryBitStreamUtil& operator=(const MemoryBitStreamUtil&);
};
