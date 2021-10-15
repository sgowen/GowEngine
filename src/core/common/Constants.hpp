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

/// Engine
#define FRAME_RATE 1.0f / 60.0f
#define MAX_NUM_PLAYERS 2

/// Input
#define MAX_TEXT_INPUT_LENGTH 16
#define MAX_NUM_SUPPORTED_GAMEPADS 2

/// Network
#define NW_CLNT_TIME_BETWEEN_HELLOS 60
#define NW_CLNT_TIMEOUT 60
#define NW_SRVR_TIMEOUT 180
#define NW_ACK_TIMEOUT 30
#define NW_CLNT_MAX_NUM_MOVES 3
#define NW_MAX_NUM_PACKETS_PER_FRAME 4
#define NW_MAX_PACKET_SIZE 1200
#define ALL_DIRTY_STATE 0xFFu

/// Audio
#define MAX_NUM_SOUNDS_TO_PLAY_PER_FRAME 3
