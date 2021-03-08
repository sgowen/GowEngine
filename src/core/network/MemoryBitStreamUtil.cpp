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
#include "core/entity/Entity.hpp"

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

void MemoryBitStreamUtil::read(InputMemoryBitStream& imbs, NetworkDataField& ndf)
{
    switch (ndf._type)
    {
        case NDFT_BOOL:
            imbs.read(ndf.valueBool());
            break;
        case NDFT_UINT8:
            imbs.read(ndf.valueUInt8());
            break;
        case NDFT_UINT16:
            imbs.read(ndf.valueUInt16());
            break;
        case NDFT_UINT32:
            imbs.read(ndf.valueUInt32());
            break;
        case NDFT_UINT64:
            imbs.read(ndf.valueUInt64());
            break;
        case NDFT_INT8:
            imbs.read(ndf.valueInt8());
            break;
        case NDFT_INT16:
            imbs.read(ndf.valueInt16());
            break;
        case NDFT_INT32:
            imbs.read(ndf.valueInt32());
            break;
        case NDFT_INT64:
            imbs.read(ndf.valueInt64());
            break;
        case NDFT_FLOAT:
            imbs.read(ndf.valueFloat());
            break;
        case NDFT_DOUBLE:
            imbs.read(ndf.valueDouble());
            break;
        case NDFT_SMALL_STRING:
            imbs.readSmall(ndf.valueString());
            break;
        case NDFT_LARGE_STRING:
            imbs.readLarge(ndf.valueString());
            break;
        default:
            break;
    }
}

void MemoryBitStreamUtil::write(OutputMemoryBitStream& ombs, NetworkDataField& ndf)
{
    switch (ndf._type)
    {
        case NDFT_BOOL:
            ombs.write(ndf.valueBool());
            break;
        case NDFT_UINT8:
            ombs.write(ndf.valueUInt8());
            break;
        case NDFT_UINT16:
            ombs.write(ndf.valueUInt16());
            break;
        case NDFT_UINT32:
            ombs.write(ndf.valueUInt32());
            break;
        case NDFT_UINT64:
            ombs.write(ndf.valueUInt64());
            break;
        case NDFT_INT8:
            ombs.write(ndf.valueInt8());
            break;
        case NDFT_INT16:
            ombs.write(ndf.valueInt16());
            break;
        case NDFT_INT32:
            ombs.write(ndf.valueInt32());
            break;
        case NDFT_INT64:
            ombs.write(ndf.valueInt64());
            break;
        case NDFT_FLOAT:
            ombs.write(ndf.valueFloat());
            break;
        case NDFT_DOUBLE:
            ombs.write(ndf.valueDouble());
            break;
        case NDFT_SMALL_STRING:
            ombs.writeSmall(ndf.valueString());
            break;
        case NDFT_LARGE_STRING:
            ombs.writeLarge(ndf.valueString());
            break;
        default:
            break;
    }
}
