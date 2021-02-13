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

void MemoryBitStreamUtil::read(InputMemoryBitStream& imbs, float& x, float& y)
{
    bool isZero;
    imbs.read(isZero);
    if (isZero)
    {
        x = 0;
    }
    else
    {
        imbs.read(x);
    }
    
    imbs.read(isZero);
    if (isZero)
    {
        y = 0;
    }
    else
    {
        imbs.read(y);
    }
}

void MemoryBitStreamUtil::write(OutputMemoryBitStream& ombs, float& x, float& y)
{
    bool isZero = x == 0;
    ombs.write(isZero);
    if (!isZero)
    {
        ombs.write(x);
    }
    
    isZero = y == 0;
    ombs.write(isZero);
    if (!isZero)
    {
        ombs.write(y);
    }
}
