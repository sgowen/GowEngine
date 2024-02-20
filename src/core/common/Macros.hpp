//
//  Macros.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

/// Bit Flag Utils
#define SET_BIT(val, bit, set) val = (val & ~bit) | (set ? bit : 0)
#define IS_BIT_SET(val, bit) (val & bit)

/// Number Utils
#define GOW_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define GOW_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define CLAMP(x, lower, upper) (GOW_MIN(upper, GOW_MAX(x, lower)))
#define DEGREES_TO_RADIANS(angle) ( (angle) / 180.0f * M_PI )
#define RADIANS_TO_DEGREES(angle) ( (angle) / M_PI * 180.0f )

/// Handy for determining num bits required given a specific value
#define NBITS2(n)  ((n&2)?1:0)
#define NBITS4(n)  ((n&(0xC))?(2+NBITS2(n>>2)):(NBITS2(n)))
#define NBITS8(n)  ((n&0xF0)?(4+NBITS4(n>>4)):(NBITS4(n)))
#define NBITS16(n) ((n&0xFF00)?(8+NBITS8(n>>8)):(NBITS8(n)))
#define NBITS32(n) ((n&0xFFFF0000)?(16+NBITS16(n>>16)):(NBITS16(n)))
#define NBITS(n)   (n==0?0:NBITS32(n)+1)

/// Handy singleton accessors
#define ASSET_HANDLER FileAssetHandler::getInstance()
#define ASSETS_MGR AssetsManager::getInstance()
#define AUDIO_ENGINE AudioEngine::getInstance()
#define FPS_UTIL FPSUtil::getInstance()
#define INST_REG InstanceRegistry::getInstance()
#define THREAD_MGR ThreadManager::getInstance()
#define ENGINE_STATE_DEFAULT DefaultEngineState::getInstance()
#define ENGINE_STATE_TITLE TitleEngineState::getInstance()
#define ENGINE_STATE_GAME_CLNT GameClientEngineState::getInstance()
#define ENGINE_STATE_GAME_HOST GameHostEngineState::getInstance()
#define ENGINE_STATE_GAME_SRVR GameServerEngineState::getInstance()
#define ENGINE_CFG EngineConfig::getInstance()
#define ENTITY_LAYOUT_MGR EntityLayoutManager::getInstance()
#define ENTITY_MGR EntityManager::getInstance()
#define INPUT_MGR InputManager::getInstance()
#define NW_CLNT NetworkClient::getInstance()
#define NW_SRVR NetworkServer::getInstance()
#define OAL OpenALUtil::getInstance()
#define OGL OpenGLUtil::getInstance()
#define SOCKET_UTIL SocketUtil::getInstance()

/// Handy static functions
#define RES_FILE_PATH(filePath) FileCompatUtil::resourceFilePath(filePath)
#define OPEN_FILE(filePath, mode) FileCompatUtil::openFile(filePath, mode)
#define LOG(...) Logger::getInstance().log(__VA_ARGS__)
#define STRING_FORMAT(...) StringUtil::format(__VA_ARGS__)

/// Miscellaneous
#define UNUSED(x) (void)(x)
#define BUFFER_OFFSET(i) ((void*)(i))

/// EntityController
#define DECL_EntityController_create          \
public:                                       \
    static EntityController* create(Entity* e)

#define IMPL_EntityController_create(type)    \
EntityController* type::create(Entity* e)     \
{                                             \
    return new type(e);                       \
}

#define IMPL_EntityController_create_NOPARENT \
IMPL_EntityController_create(EntityController)

/// EntityNetworkController
#define DECL_EntityNetworkController_create                \
public:                                                    \
    static EntityNetworkController* create(Entity* e)

#define IMPL_EntityNetworkController_create(type)          \
EntityNetworkController* type::create(Entity* e)           \
{                                                          \
    return new type(e);                                    \
}

#define IMPL_EntityNetworkController_create_NOPARENT       \
IMPL_EntityNetworkController_create(EntityNetworkController)

/// EntityRenderController
#define DECL_EntityRenderController_create               \
public:                                                  \
    static EntityRenderController* create(Entity* e)

#define IMPL_EntityRenderController_create(type)         \
EntityRenderController* type::create(Entity* e)          \
{                                                        \
    return new type(e);                                  \
}

#define IMPL_EntityRenderController_create_NOPARENT      \
IMPL_EntityRenderController_create(EntityRenderController)
