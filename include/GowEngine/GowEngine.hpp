//
//  GowEngine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/28/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

/// system
#include <algorithm>
#include <cassert>
#include <ctime>
#define _USE_MATH_DEFINES
#include <math.h>
#include <memory>
#include <set>
#include <sstream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

/// compiler variables
#include "BuildConstants.hpp"

/// deps/box2d Implementation
#include "deps/box2d/Box2DPhysicsController.hpp"
#include "deps/box2d/Box2DPhysicsRenderer.hpp"
#include "deps/box2d/Box2DPhysicsWorld.hpp"

/// deps/file-compat Implementation
#include "deps/file-compat/FileCompatUtil.hpp"

/// deps/libsndfile implementation
#include "deps/libsndfile/SoundLoader.hpp"

/// deps/rapidjson implementation
#include "deps/rapidjson/AssetsLoader.hpp"
#include "deps/rapidjson/ConfigLoader.hpp"
#include "deps/rapidjson/EntityLayoutManagerLoader.hpp"
#include "deps/rapidjson/EntityManagerLoader.hpp"
#include "deps/rapidjson/RapidJSONUtil.hpp"
#include "deps/rapidjson/RendererLoader.hpp"

/// deps/stb implementation
#include "deps/stb/TextureLoader.hpp"

/// deps/steamworks_sdk implementation
#include "deps/steamworks_sdk/Steam.hpp"
#include "deps/steamworks_sdk/SteamAddress.hpp"
#include "deps/steamworks_sdk/SteamClientHelper.hpp"
#include "deps/steamworks_sdk/SteamClientHelper.hpp"
#include "deps/steamworks_sdk/SteamServerInfo.hpp"
#include "deps/steamworks_sdk/SteamGameServices.hpp"
#include "deps/steamworks_sdk/SteamP2PAuth.hpp"
#include "deps/steamworks_sdk/SteamPacketHandler.hpp"
#include "deps/steamworks_sdk/SteamServerHelper.hpp"

/// core/assets
#include "core/assets/Animation.hpp"
#include "core/assets/AssetHandler.hpp"
#include "core/assets/Assets.hpp"
#include "core/assets/AssetsManager.hpp"
#include "core/assets/EmbeddedAssetHandler.hpp"
#include "core/assets/FileAssetHandler.hpp"
#include "core/assets/FileData.hpp"
#include "core/assets/Font.hpp"
#include "core/assets/ImageView.hpp"
#include "core/assets/MatrixDescriptor.hpp"
#include "core/assets/ShaderDescriptor.hpp"
#include "core/assets/ShaderLoader.hpp"
#include "core/assets/SoundDescriptor.hpp"
#include "core/assets/TextureDescriptor.hpp"
#include "core/assets/TextureRegion.hpp"
#include "core/assets/TextView.hpp"

/// core/common
#include "core/common/Config.hpp"
#include "core/common/Constants.hpp"
#include "core/common/FPSUtil.hpp"
#include "core/common/FixedSizePool.hpp"
#include "core/common/InstanceRegistry.hpp"
#include "core/common/Logger.hpp"
#include "core/common/Macros.hpp"
#include "core/common/Pool.hpp"
#include "core/common/RTTI.hpp"
#include "core/common/STLUtil.hpp"
#include "core/common/StateMachine.hpp"
#include "core/common/StringUtil.hpp"
#include "core/common/ThreadManager.hpp"
#include "core/common/TimeTracker.hpp"
#include "core/common/TimeUtil.hpp"

/// core/engine
#include "core/engine/DefaultEngineState.hpp"
#include "core/engine/Engine.hpp"
#include "core/engine/EngineConfig.hpp"
#include "core/engine/EngineState.hpp"
#include "core/engine/NosGameEngineState.hpp"
#include "core/engine/NosServer.hpp"
#include "core/engine/TitleEngineState.hpp"

/// core/entity
#include "core/entity/Entity.hpp"
#include "core/entity/EntityController.hpp"
#include "core/entity/EntityIDManager.hpp"
#include "core/entity/EntityLayoutManager.hpp"
#include "core/entity/EntityManager.hpp"
#include "core/entity/EntityNetworkController.hpp"
#include "core/entity/EntityPhysicsController.hpp"
#include "core/entity/EntityRegistry.hpp"
#include "core/entity/EntityRenderController.hpp"
#include "core/entity/JonController.hpp"

/// core/input
#include "core/input/ClipboardHandler.hpp"
#include "core/input/CursorEvent.hpp"
#include "core/input/GamepadEvent.hpp"
#include "core/input/InputManager.hpp"
#include "core/input/InputProcessor.hpp"
#include "core/input/InputState.hpp"
#include "core/input/KeyboardEvent.hpp"

/// core/math
#include "core/math/Circle.hpp"
#include "core/math/Line.hpp"
#include "core/math/MathUtil.hpp"
#include "core/math/OverlapTester.hpp"
#include "core/math/Rektangle.hpp"
#include "core/math/Triangle.hpp"
#include "core/math/Vector2.hpp"

/// core/network
#include "core/network/AckRange.hpp"
#include "core/network/ClientHelper.hpp"
#include "core/network/ClientProxy.hpp"
#include "core/network/DeliveryNotificationManager.hpp"
#include "core/network/InFlightPacket.hpp"
#include "core/network/InputMemoryBitStream.hpp"
#include "core/network/MachineAddress.hpp"
#include "core/network/MemoryBitStreamUtil.hpp"
#include "core/network/Move.hpp"
#include "core/network/MoveList.hpp"
#include "core/network/MovingAverage.hpp"
#include "core/network/Network.hpp"
#include "core/network/NetworkClient.hpp"
#include "core/network/NetworkHelper.hpp"
#include "core/network/NetworkServer.hpp"
#include "core/network/OutputMemoryBitStream.hpp"
#include "core/network/PacketHandler.hpp"
#include "core/network/ReplicationCommand.hpp"
#include "core/network/ReplicationManagerClient.hpp"
#include "core/network/ReplicationManagerServer.hpp"
#include "core/network/ReplicationTransmissionData.hpp"
#include "core/network/ServerHelper.hpp"
#include "core/network/SocketAddress.hpp"
#include "core/network/SocketAddressFactory.hpp"
#include "core/network/SocketClientHelper.hpp"
#include "core/network/SocketPacketHandler.hpp"
#include "core/network/SocketServerHelper.hpp"
#include "core/network/SocketUtil.hpp"
#include "core/network/TransmissionData.hpp"
#include "core/network/UDPSocket.hpp"

/// core/openal
#include "core/openal/AudioEngine.hpp"
#include "core/openal/OpenALUtil.hpp"
#include "core/openal/Sound.hpp"
#include "core/openal/SoundManager.hpp"

/// core/opengl
#include "core/opengl/CircleBatcher.hpp"
#include "core/opengl/Color.hpp"
#include "core/opengl/FontBatcher.hpp"
#include "core/opengl/Framebuffer.hpp"
#include "core/opengl/FramebufferRenderer.hpp"
#include "core/opengl/LightRenderer.hpp"
#include "core/opengl/LineBatcher.hpp"
#include "core/opengl/Matrix.hpp"
#include "core/opengl/OpenGLUtil.hpp"
#include "core/opengl/RektangleBatcher.hpp"
#include "core/opengl/Renderer.hpp"
#include "core/opengl/ScreenRenderer.hpp"
#include "core/opengl/Shader.hpp"
#include "core/opengl/ShaderAttribute.hpp"
#include "core/opengl/ShaderInput.hpp"
#include "core/opengl/ShaderManager.hpp"
#include "core/opengl/ShaderUniform.hpp"
#include "core/opengl/ShockwaveRenderer.hpp"
#include "core/opengl/SpriteBatcher.hpp"
#include "core/opengl/Texture.hpp"
#include "core/opengl/TextureManager.hpp"
#include "core/opengl/TriangleBatcher.hpp"

/// core/physics
#include "core/physics/NosPhysicsController.hpp"
#include "core/physics/NosPhysicsRenderer.hpp"
#include "core/physics/NosPhysicsWorld.hpp"
#include "core/physics/World.hpp"
