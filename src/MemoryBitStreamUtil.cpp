//
//  MemoryBitStreamUtil.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/17/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "MemoryBitStreamUtil.hpp"

#include "InputMemoryBitStream.hpp"
#include "OutputMemoryBitStream.hpp"

void MemoryBitStreamUtil::read(InputMemoryBitStream& ip, float& x, float& y)
{
    bool isZero;
    ip.read(isZero);
    if (isZero)
    {
        x = 0;
    }
    else
    {
        ip.read(x);
    }
    
    ip.read(isZero);
    if (isZero)
    {
        y = 0;
    }
    else
    {
        ip.read(y);
    }
}

void MemoryBitStreamUtil::write(OutputMemoryBitStream& op, float& x, float& y)
{
    bool isZero = x == 0;
    op.write(isZero);
    if (!isZero)
    {
        op.write(x);
    }
    
    isZero = y == 0;
    op.write(isZero);
    if (!isZero)
    {
        op.write(y);
    }
}
