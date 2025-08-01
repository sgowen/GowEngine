//
//  NetworkServer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/Pool.hpp"
#include "core/entity/EntityRegistry.hpp"

#include <map>
#include <string>

struct TimeTracker;
class ReplicationTransmissionData;
class InputMemoryBitStream;
class OutputMemoryBitStream;
class DeliveryNotificationManager;
class MachineAddress;
class ClientProxy;
class InputState;
class Entity;
class ServerHelper;

#define NW_SRVR_CALLBACKS NetworkServer::sProcessPacket, NetworkServer::sGetClientProxy, NetworkServer::sHandleClientDisconnected

typedef void (*HandleNewClientFunc)(std::string playerName, uint8_t playerID);
typedef void (*HandleLostClientFunc)(ClientProxy& cp, uint8_t localPlayerIndex);

class NetworkServer
{
public:
    static void create(ServerHelper* serverHelper, EntityIDManager& eidm, TimeTracker& tt, OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf, HandleNewClientFunc hncf, HandleLostClientFunc hlcf);
    static NetworkServer* getInstance();
    static void destroy();
    
    static void sProcessPacket(InputMemoryBitStream& inInputStream, MachineAddress* inFromAddress);
    static ClientProxy* sGetClientProxy(uint8_t inPlayerId);
    static void sHandleClientDisconnected(ClientProxy* inClientProxy);
    
    int processIncomingPackets();
    void sendOutgoingPackets();
    void registerEntity(Entity* e);
    void registerNewEntity(uint32_t key, uint32_t x, uint32_t y);
    void deregisterEntity(Entity* e);
    void deregisterAllEntities();
    void setStateDirty(uint32_t networkID, uint8_t dirtyState);
    ClientProxy* getClientProxy(uint8_t playerID) const;
    uint8_t getNumClientsConnected();
    uint8_t getNumPlayersConnected();
    MachineAddress* getServerAddress();
    bool isConnected();
    bool connect();
    void onEntityRegistered(Entity* e);
    void onEntityDeregistered(Entity* e);
    void processPacket(InputMemoryBitStream& imbs, MachineAddress* fromAddress);
    void removeProcessedMoves();
    void setNumMovesProcessed(uint32_t numMovesProcessed);
    const std::map<int, ClientProxy*>& playerIDToClientMap();
    ServerHelper* getServerHelper();
    
private:
    static NetworkServer* s_instance;
    
    ServerHelper* _serverHelper;
    EntityIDManager& _entityIDManager;
    TimeTracker& _timeTracker;
    OnEntityRegisteredFunc _onEntityRegisteredFunc;
    OnEntityDeregisteredFunc _onEntityDeregisteredFunc;
    HandleNewClientFunc _handleNewClientFunc;
    HandleLostClientFunc _handleLostClientFunc;
    EntityRegistry _entityRegistry;
    Pool<ReplicationTransmissionData> _poolRMTD;
    Pool<InputState> _poolInputState;
    std::map<uint64_t, ClientProxy> _addressHashToClientMap;
    std::map<int, ClientProxy*> _playerIDToClientMap;
    uint8_t _nextPlayerID;
    uint32_t _numMovesProcessed;
    
    int getNumMovesReadyToBeProcessed();
    void sendPacket(const OutputMemoryBitStream& ombs, MachineAddress* fromAddress);
    void handlePacketFromNewClient(InputMemoryBitStream& imbs, MachineAddress* fromAddress);
    void processPacket(ClientProxy& cp, InputMemoryBitStream& imbs);
    void sendWelcomePacket(ClientProxy& cp);
    void sendLocalPlayerAddedPacket(ClientProxy& cp);
    void sendStatePacketToClient(ClientProxy& cp);
    void handleInputPacket(ClientProxy& cp, InputMemoryBitStream& imbs);
    void handleAddLocalPlayerPacket(ClientProxy& cp, InputMemoryBitStream& imbs);
    void handleDropLocalPlayerPacket(ClientProxy& cp, InputMemoryBitStream& imbs);
    void handleClientDisconnected(ClientProxy& cp);
    void resetNextPlayerID();
    
    NetworkServer(ServerHelper* serverHelper, EntityIDManager& eidm, TimeTracker& tt, OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf, HandleNewClientFunc hncf, HandleLostClientFunc hlcf);
    ~NetworkServer();
    NetworkServer(const NetworkServer&);
    NetworkServer& operator=(const NetworkServer&);
};
