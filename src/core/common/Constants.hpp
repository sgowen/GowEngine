//
//  Constants.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 10/14/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

// FIXME
// Just create a global engine config singleton
// This file is a good starting place
// But ultimately tweaking things like this
// shouldn't have to be a full recompile
// so just put it all in a json file
// Exceptions to this are things like frameRate...
// The whole game is built for a specific frameRate
// I don't see it getting tweaked
// Same with max num players.
// Upping NW_CLNT_MAX_NUM_MOVES would cause a crash
// Since we are only using 2 bits to encode it.
// So some of these values would need to move to NosFURa2 Constants.hpp
// Some could stay here
// The rest could be loaded up via global engine config

/// Engine
#define FRAME_RATE 1.0f / 60.0f
#define MAX_NUM_PLAYERS 2

/// Input
#define MAX_TEXT_INPUT_LENGTH 16
#define MAX_NUM_SUPPORTED_GAMEPADS 2

/// Network
#define NW_CLIENT_PORT_HOST 1337
#define NW_CLIENT_PORT_JOIN 1338
#define NW_SERVER_PORT 9999
#define NW_CLNT_TIME_BETWEEN_HELLOS 60
#define NW_CLNT_TIMEOUT 60
#define NW_SRVR_TIMEOUT 180
#define NW_ACK_TIMEOUT 30
#define NW_CLNT_MAX_NUM_MOVES 7
#define NW_MAX_NUM_PACKETS_PER_FRAME 4
#define NW_MAX_PACKET_SIZE 1200
#define NW_ALL_DIRTY_STATE 0xFFu

/// Audio
#define MAX_NUM_SOUNDS_TO_PLAY_PER_FRAME 3
