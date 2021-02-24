//
//  NetworkServer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Pool.hpp"
#include "EntityRegistry.hpp"
#include "PacketHandler.hpp"

#include <map>
#include <string>

class ReplicationManagerTransmissionData;
class InputMemoryBitStream;
class OutputMemoryBitStream;
class DeliveryNotificationManager;
class SocketAddress;
class ClientProxy;
class InputState;
class Entity;

#define NW_SRVR (NetworkServer::getInstance())

typedef void (*HandleNewClientFunc)(std::string username, uint8_t playerID);
typedef void (*HandleLostClientFunc)(ClientProxy& cp, uint8_t localPlayerIndex);
typedef InputState* (*InputStateCreationFunc)();
typedef void (*InputStateReleaseFunc)(InputState* inputState);

class NetworkServer
{
public:
    static void create(uint16_t port, uint8_t maxNumPlayers, OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf, HandleNewClientFunc hncf, HandleLostClientFunc hlcf, InputStateCreationFunc iscf, InputStateReleaseFunc isrf);
    static NetworkServer* getInstance();
    static void destroy();
    
    void processIncomingPackets();
    void sendOutgoingPackets();
    void registerEntity(Entity* e);
    void deregisterEntity(Entity* e);
    void deregisterAllEntities();
    void setStateDirty(uint32_t networkID, uint8_t dirtyState);
    ClientProxy* getClientProxy(uint8_t playerID) const;
    int getMoveCount();
    uint8_t getNumClientsConnected();
    uint8_t getNumPlayersConnected();
    SocketAddress& getServerAddress();
    bool connect();
    void onEntityRegistered(Entity* e);
    void onEntityDeregistered(Entity* e);
    EntityRegistry& getEntityRegistry();
    void processPacket(InputMemoryBitStream& imbs, SocketAddress* fromAddress);
    void onMovesProcessed(uint8_t moveCount);
    uint32_t getNumMovesProcessed();
    const std::map<int, ClientProxy*>& playerIDToClientMap();
    
private:
    static NetworkServer* s_instance;
    
    PacketHandler _packetHandler;
    OnEntityRegisteredFunc _onEntityRegisteredFunc;
    OnEntityDeregisteredFunc _onEntityDeregisteredFunc;
    HandleNewClientFunc _handleNewClientFunc;
    HandleLostClientFunc _handleLostClientFunc;
    InputStateCreationFunc _inputStateCreationFunc;
    InputStateReleaseFunc _inputStateReleaseFunc;
    EntityRegistry _entityRegistry;
    Pool<ReplicationManagerTransmissionData> _poolRMTD;
    std::map<uint64_t, ClientProxy> _addressHashToClientMap;
    std::map<int, ClientProxy*> _playerIDToClientMap;
    uint8_t _nextPlayerID;
    uint8_t _maxNumPlayers;
    uint32_t _numMovesProcessed;
    uint16_t _port;
    
    void sendPacket(const OutputMemoryBitStream& ombs, SocketAddress* fromAddress);
    void handlePacketFromNewClient(InputMemoryBitStream& imbs, SocketAddress* fromAddress);
    void processPacket(ClientProxy& cp, InputMemoryBitStream& imbs);
    void sendWelcomePacket(ClientProxy& cp);
    void sendLocalPlayerAddedPacket(ClientProxy& cp);
    void sendStatePacketToClient(ClientProxy& cp);
    void handleInputPacket(ClientProxy& cp, InputMemoryBitStream& imbs);
    void handleAddLocalPlayerPacket(ClientProxy& cp, InputMemoryBitStream& imbs);
    void handleDropLocalPlayerPacket(ClientProxy& cp, InputMemoryBitStream& imbs);
    void handleClientDisconnected(ClientProxy& cp);
    void resetNextPlayerID();
    
    NetworkServer(uint16_t port, uint8_t maxNumPlayers, OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf, HandleNewClientFunc hncf, HandleLostClientFunc hlcf, InputStateCreationFunc iscf, InputStateReleaseFunc isrf);
    ~NetworkServer();
    NetworkServer(const NetworkServer&);
    NetworkServer& operator=(const NetworkServer&);
};
