//
//  Constants.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/3/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

/// Game Engine State Args
#define ARG_IS_HOST "ARG_IS_HOST"
#define ARG_IP_ADDRESS "ARG_IP_ADDRESS"
#define ARG_USERNAME "ARG_USERNAME"
#define ARG_STEAM_ADDRESS "ARG_STEAM_ADDRESS"

/// Engine
#define FILE_PATH_ENGINE_ASSETS "engine/json/default/Assets.json"
#define FILE_PATH_ENGINE_CONFIG "engine/json/Config.json"
#define FILE_PATH_ENGINE_RENDERER "engine/json/default/Renderer.json"
#define FILE_PATH_SCRIPT_DEFAULT_RENDERER "engine/scripts/DefaultRenderer.lua"
#define FILE_PATH_SHADER_FRAGMENT_FRAMEBUFFER "engine/shaders/framebuffer.fsh"
#define FILE_PATH_SHADER_VERTEX_FRAMEBUFFER "engine/shaders/framebuffer.vsh"
#define FILE_PATH_SHADER_FRAGMENT_GEOMETRY "engine/shaders/geometry.fsh"
#define FILE_PATH_SHADER_VERTEX_GEOMETRY "engine/shaders/geometry.vsh"
#define FILE_PATH_SHADER_FRAGMENT_LIGHTS "engine/shaders/lights.fsh"
#define FILE_PATH_SHADER_VERTEX_LIGHTS "engine/shaders/lights.vsh"
#define FILE_PATH_SHADER_FRAGMENT_SHOCKWAVE "engine/shaders/shockwave.fsh"
#define FILE_PATH_SHADER_VERTEX_SHOCKWAVE "engine/shaders/shockwave.vsh"
#define FILE_PATH_SHADER_FRAGMENT_SPRITE "engine/shaders/sprite.fsh"
#define FILE_PATH_SHADER_VERTEX_SPRITE "engine/shaders/sprite.vsh"
#define FILE_PATH_TEXTURE_FONT "engine/textures/sf_font.png"
#define MAX_NUM_PLAYERS 4

/// Entity ID
#define  LAYOUT_ENTITY_ID_BEGIN 0x00000000u
#define  LAYOUT_ENTITY_ID_END   0x000FFFFFu
#define NETWORK_ENTITY_ID_BEGIN LAYOUT_ENTITY_ID_END
#define NETWORK_ENTITY_ID_END   0x00FFFFFFu
#define PLAYER_ENTITY_ID_BEGIN  NETWORK_ENTITY_ID_END
#define PLAYER_ENTITY_ID_END    0xFFFFFFFFu

/// Input
#define INACTIVE_PLAYER_ID 0
#define NO_INPUT 0

/// Rendering
#define CIRCLE_DEGREE_SPACING 6
#define NUM_VERTICES_PER_CIRCLE 360 / CIRCLE_DEGREE_SPACING
#define NUM_SUPPORTED_LIGHTS 12
#define NUM_VERTICES_PER_LINE 2
#define NUM_VERTICES_PER_TRIANGLE 3
#define NUM_VERTICES_PER_RECTANGLE 4
#define NUM_INDICES_PER_RECTANGLE 6
#define NUM_SUPPORTED_TEXTURE_SLOTS 4

/// Colors
#define COLOR_CLEAR 0.0f, 0.0f, 0.0f, 0.0f
#define COLOR_WHITE 1.0f, 1.0f, 1.0f, 1.0f
#define COLOR_BLACK 0.0f, 0.0f, 0.0f, 1.0f
#define COLOR_RED 1.0f, 0.0f, 0.0f, 1.0f
#define COLOR_GREEN 0.0f, 1.0f, 0.0f, 1.0f
#define COLOR_BLUE 0.0f, 0.0f, 1.0f, 1.0f
#define COLOR_HALF 0.5f, 0.5f, 0.5f, 0.5f
#define COLOR_DIM 0.1f, 0.1f, 0.1f, 0.9f
#define COLOR_DOUBLE 2.0f, 2.0f, 2.0f, 1.0f
#define COLOR_ONE 1.0f, 1.0f, 1.0f, 1.0f

/// Vector
#define VECTOR2_ZERO 0.0f, 0.0f

/// Network
#define NW_MAX_PACKET_SIZE 508
#define NW_ALL_DIRTY_STATE 0xFFu

/// Steam
#define STEAM_SERVER_PORT 27015
#define STEAM_MASTER_SERVER_UPDATER_PORT 27016

/// Keyboard
#define MAX_TEXT_INPUT_LENGTH 16
#define GOW_KEY_0 0x0030
#define GOW_KEY_1 0x0031
#define GOW_KEY_2 0x0032
#define GOW_KEY_3 0x0033
#define GOW_KEY_4 0x0034
#define GOW_KEY_5 0x0035
#define GOW_KEY_6 0x0036
#define GOW_KEY_7 0x0037
#define GOW_KEY_8 0x0038
#define GOW_KEY_9 0x0039
#define GOW_KEY_A 0x0041
#define GOW_KEY_B 0x0042
#define GOW_KEY_C 0x0043
#define GOW_KEY_D 0x0044
#define GOW_KEY_E 0x0045
#define GOW_KEY_F 0x0046
#define GOW_KEY_G 0x0047
#define GOW_KEY_H 0x0048
#define GOW_KEY_I 0x0049
#define GOW_KEY_J 0x004A
#define GOW_KEY_K 0x004B
#define GOW_KEY_L 0x004C
#define GOW_KEY_M 0x004D
#define GOW_KEY_N 0x004E
#define GOW_KEY_O 0x004F
#define GOW_KEY_P 0x0050
#define GOW_KEY_Q 0x0051
#define GOW_KEY_R 0x0052
#define GOW_KEY_S 0x0053
#define GOW_KEY_T 0x0054
#define GOW_KEY_U 0x0055
#define GOW_KEY_V 0x0056
#define GOW_KEY_W 0x0057
#define GOW_KEY_X 0x0058
#define GOW_KEY_Y 0x0059
#define GOW_KEY_Z 0x005A
#define GOW_KEY_NEW_LINE 0x0A
#define GOW_KEY_SPACE_BAR 0x0020
#define GOW_KEY_COMMA 0x002C
#define GOW_KEY_PERIOD 0x002E
#define GOW_KEY_CTRL 341
#define GOW_KEY_CMD 343
#define GOW_KEY_DELETE 0x007F
#define GOW_KEY_BACK_SPACE 0x0103
#define GOW_KEY_ESCAPE 0x0100
#define GOW_KEY_ANDROID_BACK_BUTTON 0x00E0
#define GOW_KEY_ARROW_LEFT 0x0107
#define GOW_KEY_ARROW_UP 0x0109
#define GOW_KEY_ARROW_RIGHT 0x0106
#define GOW_KEY_ARROW_DOWN 0x0108
#define GOW_KEY_CARRIAGE_RETURN 0x0101
