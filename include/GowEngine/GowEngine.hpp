//
//  GowEngine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/28/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

/// System
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

/// glfw Implementation
#include "deps/glfw/GlfwEngine.hpp"

/// libsndfile Implementation
#include "deps/libsndfile/SoundLoader.hpp"

/// rapidjson Implementation
#include "deps/rapidjson/AssetsLoader.hpp"
#include "deps/rapidjson/ConfigLoader.hpp"
#include "deps/rapidjson/EntityLayoutManagerLoader.hpp"
#include "deps/rapidjson/EntityManagerLoader.hpp"
#include "deps/rapidjson/RapidJSONUtil.hpp"
#include "deps/rapidjson/RendererLoader.hpp"

/// stb Implementation
#include "deps/stb/TextureLoader.hpp"

/// Core Assets
#include "core/assets/AndroidAssetHandler.hpp"
#include "core/assets/AppleAssetHandler.hpp"
#include "core/assets/AppleBundleUtil.hpp"
#include "core/assets/AssetHandler.hpp"
#include "core/assets/AssetHandlerFactory.hpp"
#include "core/assets/Assets.hpp"
#include "core/assets/AssetsManager.hpp"
#include "core/assets/FileData.hpp"
#include "core/assets/FileUtil.hpp"
#include "core/assets/Font.hpp"
#include "core/assets/LinuxAssetHandler.hpp"
#include "core/assets/MatrixDescriptor.hpp"
#include "core/assets/ShaderDescriptor.hpp"
#include "core/assets/SoundDescriptor.hpp"
#include "core/assets/TextureDescriptor.hpp"
#include "core/assets/WindowsAssetHandler.hpp"

/// Core Common
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

/// Core Engine
#include "core/engine/DefaultEngineState.hpp"
#include "core/engine/Engine.hpp"
#include "core/engine/EngineConfig.hpp"
#include "core/engine/EngineState.hpp"
#include "core/engine/GameClientEngineState.hpp"
#include "core/engine/GameHostEngineState.hpp"
#include "core/engine/GameServerEngineState.hpp"
#include "core/engine/TitleEngineState.hpp"

/// Core Entity
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

/// Core Input
#include "core/input/ClipboardHandler.hpp"
#include "core/input/CursorEvent.hpp"
#include "core/input/GamepadEvent.hpp"
#include "core/input/InputManager.hpp"
#include "core/input/InputProcessor.hpp"
#include "core/input/InputState.hpp"
#include "core/input/KeyboardEvent.hpp"

/// Core Math
#include "core/math/Circle.hpp"
#include "core/math/Line.hpp"
#include "core/math/MathUtil.hpp"
#include "core/math/OverlapTester.hpp"
#include "core/math/Rektangle.hpp"
#include "core/math/Triangle.hpp"
#include "core/math/Vector2.hpp"

/// Core Network
#include "core/network/AckRange.hpp"
#include "core/network/ClientProxy.hpp"
#include "core/network/DeliveryNotificationManager.hpp"
#include "core/network/InFlightPacket.hpp"
#include "core/network/InputMemoryBitStream.hpp"
#include "core/network/MemoryBitStreamUtil.hpp"
#include "core/network/Move.hpp"
#include "core/network/MoveList.hpp"
#include "core/network/MovingAverage.hpp"
#include "core/network/Network.hpp"
#include "core/network/NetworkClient.hpp"
#include "core/network/NetworkServer.hpp"
#include "core/network/OutputMemoryBitStream.hpp"
#include "core/network/PacketHandler.hpp"
#include "core/network/ReplicationCommand.hpp"
#include "core/network/ReplicationManagerClient.hpp"
#include "core/network/ReplicationManagerServer.hpp"
#include "core/network/ReplicationTransmissionData.hpp"
#include "core/network/SocketAddress.hpp"
#include "core/network/SocketAddressFactory.hpp"
#include "core/network/SocketUtil.hpp"
#include "core/network/TransmissionData.hpp"
#include "core/network/UDPSocket.hpp"

/// Core OpenAL
#include "core/openal/AudioEngine.hpp"
#include "core/openal/OpenALUtil.hpp"
#include "core/openal/Sound.hpp"
#include "core/openal/SoundManager.hpp"

/// Core OpenGL
#include "core/opengl/Animation.hpp"
#include "core/opengl/CircleBatcher.hpp"
#include "core/opengl/Color.hpp"
#include "core/opengl/FontBatcher.hpp"
#include "core/opengl/Framebuffer.hpp"
#include "core/opengl/FramebufferRenderer.hpp"
#include "core/opengl/LightRenderer.hpp"
#include "core/opengl/LineBatcher.hpp"
#include "core/opengl/Matrix.hpp"
#include "core/opengl/NosPhysicsRenderer.hpp"
#include "core/opengl/OpenGLUtil.hpp"
#include "core/opengl/RektangleBatcher.hpp"
#include "core/opengl/Renderer.hpp"
#include "core/opengl/ScreenRenderer.hpp"
#include "core/opengl/Shader.hpp"
#include "core/opengl/ShaderAttribute.hpp"
#include "core/opengl/ShaderInput.hpp"
#include "core/opengl/ShaderLoader.hpp"
#include "core/opengl/ShaderManager.hpp"
#include "core/opengl/ShaderUniform.hpp"
#include "core/opengl/ShockwaveRenderer.hpp"
#include "core/opengl/SpriteBatcher.hpp"
#include "core/opengl/TextView.hpp"
#include "core/opengl/Texture.hpp"
#include "core/opengl/TextureManager.hpp"
#include "core/opengl/TextureRegion.hpp"
#include "core/opengl/TriangleBatcher.hpp"

/// Core Physics
#include "core/physics/NosPhysicsController.hpp"
#include "core/physics/NosPhysicsWorld.hpp"
#include "core/physics/World.hpp"
