//
//  Network.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildConstants.hpp>

#include <stdint.h>

#if IS_WINDOWS
    #include <winsock2.h>
    #include <ws2tcpip.h>

    #pragma comment(lib, "Ws2_32.lib")

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

    #define ARRAYSIZE(x) (sizeof(x) / sizeof(x[0]))
#endif

enum NetworkPacketType
{
    NWPT_HELLO = 0x01u,
    NWPT_WELCOME = 0x02u,
    NWPT_STATE = 0x03u,
    NWPT_INPUT = 0x04u,
    NWPT_ADD_LOCAL_PLAYER = 0x05u,
    NWPT_DROP_LOCAL_PLAYER = 0x06u,
    NWPT_LOCAL_PLAYER_ADDED = 0x07u,
    NWPT_LOCAL_PLAYER_DENIED = 0x08u,
    NWPT_CLNT_EXIT = 0x09u,
    NWPT_SRVR_EXIT = 0x0Au
};

enum SocketAddressFamily
{
    INET = AF_INET,
    INET6 = AF_INET6
};

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

inline double hton_double(double value)
{
    union v
    {
        double d;
        uint64_t i;
    };

    union v val;

    val.d = value;
    val.i = htonll(val.i);

    return val.d;
}

inline double ntoh_double(double value)
{
    union v
    {
        double d;
        uint64_t i;
    };

    union v val;

    val.d = value;
    val.i = ntohll(val.i);

    return val.d;
}

inline float hton_float(float value)
{
    union v
    {
        float f;
        uint32_t i;
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
        uint32_t i;
    };

    union v val;

    val.f = value;
    val.i = ntohl(val.i);

    return val.f;
}
