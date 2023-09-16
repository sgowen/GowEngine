//
//  MemoryBitStreamUtil.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/17/18.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

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
            imbs.read(ndf.refBool());
            break;
        case NDFT_UINT8:
            imbs.read(ndf.refUInt8());
            break;
        case NDFT_UINT16:
            imbs.read(ndf.refUInt16());
            break;
        case NDFT_UINT32:
            imbs.read(ndf.refUInt32());
            break;
        case NDFT_UINT64:
            imbs.read(ndf.refUInt64());
            break;
        case NDFT_INT8:
            imbs.read(ndf.refInt8());
            break;
        case NDFT_INT16:
            imbs.read(ndf.refInt16());
            break;
        case NDFT_INT32:
            imbs.read(ndf.refInt32());
            break;
        case NDFT_INT64:
            imbs.read(ndf.refInt64());
            break;
        case NDFT_FLOAT:
            imbs.read(ndf.refFloat());
            break;
        case NDFT_DOUBLE:
            imbs.read(ndf.refDouble());
            break;
        case NDFT_SMALL_STRING:
            imbs.readSmall(ndf.refString());
            break;
        case NDFT_LARGE_STRING:
            imbs.readLarge(ndf.refString());
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
            ombs.write(ndf.refBool());
            break;
        case NDFT_UINT8:
            ombs.write(ndf.refUInt8());
            break;
        case NDFT_UINT16:
            ombs.write(ndf.refUInt16());
            break;
        case NDFT_UINT32:
            ombs.write(ndf.refUInt32());
            break;
        case NDFT_UINT64:
            ombs.write(ndf.refUInt64());
            break;
        case NDFT_INT8:
            ombs.write(ndf.refInt8());
            break;
        case NDFT_INT16:
            ombs.write(ndf.refInt16());
            break;
        case NDFT_INT32:
            ombs.write(ndf.refInt32());
            break;
        case NDFT_INT64:
            ombs.write(ndf.refInt64());
            break;
        case NDFT_FLOAT:
            ombs.write(ndf.refFloat());
            break;
        case NDFT_DOUBLE:
            ombs.write(ndf.refDouble());
            break;
        case NDFT_SMALL_STRING:
            ombs.writeSmall(ndf.refString());
            break;
        case NDFT_LARGE_STRING:
            ombs.writeLarge(ndf.refString());
            break;
        default:
            break;
    }
}
