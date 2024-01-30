//
//  Steam.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/13/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildConstants.hpp>
#if IS_DESKTOP

#if IS_WINDOWS
typedef __int16 int16;
typedef unsigned __int16 uint16;
typedef __int32 int32;
typedef unsigned __int32 uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;
#else
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;
typedef uint32 DWORD;
typedef DWORD HWND;
typedef DWORD HINSTANCE;
typedef short SHORT;
typedef long LONG;
typedef unsigned char byte;
typedef unsigned char uint8;
#endif

#include <stddef.h>

#include <steam/steam_api.h>
#include <steam/steamtypes.h>
#include <steam/isteamuserstats.h>
#include <steam/isteamremotestorage.h>
#include <steam/isteammatchmaking.h>
#include <steam/steam_gameserver.h>

#ifdef STEAM_CEG
// Steam DRM header file
#include <cegclient.h>
#else
#define Steamworks_InitCEGLibrary() (true)
#define Steamworks_TermCEGLibrary()
#define Steamworks_TestSecret()
#define Steamworks_SelfCheck()
#endif

// These numbers have no meaning, I made them up, because I need them!
const int STEAM_GAME_SERVER_INIT_FAILED = 95;
const int STEAM_GAME_SERVER_INTERFACE_INVALID = 96;

// Steam Auth Network message types
enum EMessage
{
    // Server messages
    k_EMsgServerBegin = 0xF0u+1,
    k_EMsgServerSendInfo = 0xF0u+2,
    k_EMsgServerFailAuthentication = 0xF0u+3,
    k_EMsgServerPassAuthentication = 0xF0u+4,
    k_EMsgServerExiting = 0xF0u+5,

    // Client messages
    k_EMsgClientBegin = 0xF0u+6,
    k_EMsgClientInitiateConnection = 0xF0u+7, // Msg from client to server when trying to connect
    k_EMsgClientBeginAuthentication = 0xF0u+8,
    k_EMsgClientLeavingServer = 0xF0u+9,

    // P2P authentication messages
    k_EMsgP2PBegin = 0xF0u+10,
    k_EMsgP2PSendingTicket = 0xF0u+11,

    // force 8-bit size enum so the wire protocol doesn't get outgrown later
    k_EForceDWORD  = 0xFFu,
};

#endif /* IS_DESKTOP */
