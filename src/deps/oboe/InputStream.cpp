//
//  InputStream.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/22/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>
#if IS_ANDROID

InputStream::InputStream(uint8_t *buff, int32_t len) :
_buffer(buff),
_bufferLen(len),
_pos(0)
{
    // Empty
}

int32_t InputStream::read(void *buff, int32_t numBytes)
{
    int32_t numAvail = _bufferLen - _pos;
    numBytes = std::min(numBytes, numAvail);

    peek(buff, numBytes);
    _pos += numBytes;
    return numBytes;
}

int32_t InputStream::peek(void *buff, int32_t numBytes)
{
    int32_t numAvail = _bufferLen - _pos;
    numBytes = std::min(numBytes, numAvail);
    memcpy(buff, _buffer + _pos, numBytes);
    return numBytes;
}

void InputStream::advance(int32_t numBytes)
{
    if (numBytes > 0)
    {
        int32_t numAvail = _bufferLen - _pos;
        _pos += std::min(numAvail, numBytes);
    }
}

int32_t InputStream::getPos()
{
    return _pos;
}

void InputStream::setPos(int32_t pos)
{
    if (pos > 0)
    {
        if (pos < _bufferLen)
        {
            _pos = pos;
        }
        else
        {
            _pos = _bufferLen - 1;
        }
    }
}

#endif /* IS_ANDROID */
