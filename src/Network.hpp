//
//  Network.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "PlatformMacros.hpp"

#if IS_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <winsock2.h>
    #include <ws2tcpip.h>

    #pragma comment (lib, "Ws2_32.lib")

    typedef int socklen_t;

    #define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <sys/types.h>
    #include <sys/select.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <errno.h>
    #include <fcntl.h>
    #include <unistd.h>

    typedef int SOCKET;
    const int NO_ERROR = 0;
    const int INVALID_SOCKET = -1;
    const int WSAECONNRESET = ECONNRESET;
    const int WSAEWOULDBLOCK = EAGAIN;
    const int SOCKET_ERROR = -1;
#endif

#define ALL_DIRTY_STATE 0xFFFFu

#ifndef htonll
#define htonll(x) \
((uint64_t)((((uint64_t)(x) & 0xff00000000000000ULL) >> 56) | \
(((uint64_t)(x) & 0x00ff000000000000ULL) >> 40) | \
(((uint64_t)(x) & 0x0000ff0000000000ULL) >> 24) | \
(((uint64_t)(x) & 0x000000ff00000000ULL) >>  8) | \
(((uint64_t)(x) & 0x00000000ff000000ULL) <<  8) | \
(((uint64_t)(x) & 0x0000000000ff0000ULL) << 24) | \
(((uint64_t)(x) & 0x000000000000ff00ULL) << 40) | \
(((uint64_t)(x) & 0x00000000000000ffULL) << 56)))
#endif

#ifndef ntohll
#define ntohll(x) \
((uint64_t)((((uint64_t)(x) & 0xff00000000000000ULL) >> 56) | \
(((uint64_t)(x) & 0x00ff000000000000ULL) >> 40) | \
(((uint64_t)(x) & 0x0000ff0000000000ULL) >> 24) | \
(((uint64_t)(x) & 0x000000ff00000000ULL) >>  8) | \
(((uint64_t)(x) & 0x00000000ff000000ULL) <<  8) | \
(((uint64_t)(x) & 0x0000000000ff0000ULL) << 24) | \
(((uint64_t)(x) & 0x000000000000ff00ULL) << 40) | \
(((uint64_t)(x) & 0x00000000000000ffULL) << 56)))
#endif

inline float hton_float(float value)
{
    union v
    {
        float f;
        unsigned int i;
    };

    union v val;

    val.f = value;
    val.i = htonl(val.i);

    return val.f;
}

inline float ntoh_float(float value)
{
    union v
    {
        float f;
        unsigned int i;
    };

    union v val;

    val.f = value;
    val.i = ntohl(val.i);

    return val.f;
}
