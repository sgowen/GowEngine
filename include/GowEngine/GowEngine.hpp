//
//  GowEngine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/28/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

/// Box2D Implementation
#include "deps/box2d/Box2DDebugRenderer.hpp"
#include "deps/box2d/Box2DPhysicsController.hpp"

/// DirectXTK Implementation
#include "deps/DirectXTK/DirectXTKAudioEngine.hpp"
#include "deps/DirectXTK/DirectXTKSound.hpp"
#include "deps/DirectXTK/DirectXTKSoundWrapper.hpp"

/// GLFW Implementation
#include "deps/glfw/GlfwMain.hpp"

/// ObjectAL-for-iPhone Implementation
#include "deps/ObjectAL-for-iPhone/ObjectALAudioEngine.hpp"
#include "deps/ObjectAL-for-iPhone/ObjectALHelper.hpp"
#include "deps/ObjectAL-for-iPhone/ObjectALSound.hpp"
#include "deps/ObjectAL-for-iPhone/ObjectALSoundWrapper.hpp"

/// Oboe Implementation
#include "deps/oboe/InputStream.hpp"
#include "deps/oboe/OboeAudioEngine.hpp"
#include "deps/oboe/OboeSound.hpp"
#include "deps/oboe/OboeSoundWrapper.hpp"
#include "deps/oboe/SampleBuffer.hpp"
#include "deps/oboe/SampleSource.hpp"
#include "deps/oboe/SimpleMultiPlayer.hpp"
#include "deps/oboe/WavChunkHeader.hpp"
#include "deps/oboe/WavFmtChunkHeader.hpp"
#include "deps/oboe/WavRIFFChunkHeader.hpp"
#include "deps/oboe/WavStreamReader.hpp"
#include "deps/oboe/WavTypes.hpp"

/// RapidJSON Implementation
#include "deps/rapidjson/AssetsLoader.hpp"
#include "deps/rapidjson/ConfigLoader.hpp"
#include "deps/rapidjson/EntityLayoutLoader.hpp"
#include "deps/rapidjson/EntityManagerLoader.hpp"
#include "deps/rapidjson/RapidJSONUtil.hpp"
#include "deps/rapidjson/RendererLoader.hpp"

/// spdlog Implementation
#include "deps/spdlog/Logger.hpp"

/// stb Implementation
#include "deps/stb/TextureLoader.hpp"

/// TinyThread++ Implementation
#include "deps/tinythreadpp/ThreadManager.hpp"

/// Core Assets
#include "core/assets/AndroidAssetHandler.hpp"
#include "core/assets/AppleAssetHandler.hpp"
#include "core/assets/AssetHandler.hpp"
#include "core/assets/AssetHandlerFactory.hpp"
#include "core/assets/Assets.hpp"
#include "core/assets/AssetsManager.hpp"
#include "core/assets/FileData.hpp"
#include "core/assets/FileUtil.hpp"
#include "core/assets/LinuxAssetHandler.hpp"
#include "core/assets/WindowsAssetHandler.hpp"

/// Core Audio
#include "core/audio/AudioEngine.hpp"
#include "core/audio/AudioEngineFactory.hpp"
#include "core/audio/Sound.hpp"
#include "core/audio/SoundDescriptor.hpp"
#include "core/audio/SoundManager.hpp"
#include "core/audio/SoundUtil.hpp"
#include "core/audio/SoundWrapper.hpp"

/// Core Common
#include "core/common/Config.hpp"
#include "core/common/FPSUtil.hpp"
#include "core/common/FixedSizePool.hpp"
#include "core/common/InstanceRegistry.hpp"
#include "core/common/Macros.hpp"
#include "core/common/PlatformMacros.hpp"
#include "core/common/Pool.hpp"
#include "core/common/RTTI.hpp"
#include "core/common/STLUtil.hpp"
#include "core/common/StateMachine.hpp"
#include "core/common/StringUtil.hpp"
#include "core/common/TimeTracker.hpp"

/// Core Engine
#include "core/engine/DefaultEngineState.hpp"
#include "core/engine/Engine.hpp"
#include "core/engine/EngineController.hpp"
#include "core/engine/EngineState.hpp"

/// Core Entity
#include "core/entity/Entity.hpp"
#include "core/entity/EntityController.hpp"
#include "core/entity/EntityIDManager.hpp"
#include "core/entity/EntityLayout.hpp"
#include "core/entity/EntityManager.hpp"
#include "core/entity/EntityNetworkController.hpp"
#include "core/entity/EntityPhysicsController.hpp"
#include "core/entity/EntityRegistry.hpp"
#include "core/entity/EntityRenderController.hpp"

/// Core Input
#include "core/input/ClipboardUtil.hpp"
#include "core/input/CursorEvent.hpp"
#include "core/input/GamepadEvent.hpp"
#include "core/input/InputManager.hpp"
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
#include "core/openal/OpenALAudioEngine.hpp"
#include "core/openal/OpenALSound.hpp"
#include "core/openal/OpenALSoundWrapper.hpp"

/// Core OpenGL
#include "core/opengl/Animation.hpp"
#include "core/opengl/CircleBatcher.hpp"
#include "core/opengl/Color.hpp"
#include "core/opengl/FontBatcher.hpp"
#include "core/opengl/Framebuffer.hpp"
#include "core/opengl/LightRenderer.hpp"
#include "core/opengl/LineBatcher.hpp"
#include "core/opengl/Matrix.hpp"
#include "core/opengl/MatrixDescriptor.hpp"
#include "core/opengl/OpenGLWrapper.hpp"
#include "core/opengl/RektangleBatcher.hpp"
#include "core/opengl/Renderer.hpp"
#include "core/opengl/ScreenRenderer.hpp"
#include "core/opengl/Shader.hpp"
#include "core/opengl/ShaderAttribute.hpp"
#include "core/opengl/ShaderDescriptor.hpp"
#include "core/opengl/ShaderInput.hpp"
#include "core/opengl/ShaderLoader.hpp"
#include "core/opengl/ShaderManager.hpp"
#include "core/opengl/ShaderUniform.hpp"
#include "core/opengl/SpriteBatcher.hpp"
#include "core/opengl/TextView.hpp"
#include "core/opengl/Texture.hpp"
#include "core/opengl/TextureDescriptor.hpp"
#include "core/opengl/TextureManager.hpp"
#include "core/opengl/TextureRegion.hpp"
#include "core/opengl/TriangleBatcher.hpp"

/// Core Physics
#include "core/physics/TopDownPhysicsController.hpp"
