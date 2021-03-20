//
//  Network.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/PlatformMacros.hpp"

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
#endif

#define ALL_DIRTY_STATE 0xFFu

enum InstanceKeys
{
    INSK_TIME_SRVR = 'TIMS',
    INSK_TIME_CLNT = 'TIMC',
    INSK_EID_SRVR = 'EIMS',
    INSK_EID_CLNT = 'EIMC',
    INSK_ELM_SRVR = 'ELMS',
    INSK_ELM_CLNT = 'ELMC'
};

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

#define NW_CLNT_TIME_BETWEEN_HELLOS 60
#define NW_CLNT_TIMEOUT 180
#define NW_SRVR_TIMEOUT 300
#define NW_ACK_TIMEOUT 30
#define NW_CLNT_MAX_NUM_MOVES 15
#define NW_MAX_NUM_PACKETS_PER_FRAME 10
#define NW_MAX_PACKET_SIZE 1200
#define NW_INPUT_UNASSIGNED 7

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
