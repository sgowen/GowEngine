//
//  InputStream.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/22/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildMacros.hpp>
#if IS_ANDROID

#include <stdint.h>

class InputStream
{
public:
    InputStream(uint8_t *buff, int32_t len);

    int32_t read(void *buff, int32_t numBytes);
    int32_t peek(void *buff, int32_t numBytes);
    void advance(int32_t numBytes);
    int32_t getPos();
    void setPos(int32_t pos);

private:
    uint8_t *_buffer;
    int32_t _bufferLen;
    int32_t _pos;
};

#endif /* IS_ANDROID */
