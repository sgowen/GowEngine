//
//  NetworkManagerServer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Pool.hpp"

#include <map>
#include <string>

class EntityManager;
class ReplicationManagerTransmissionData;
class ServerHelper;
class InputMemoryBitStream;
class OutputMemoryBitStream;
class DeliveryNotificationManager;
class MachineAddress;
class ClientProxy;
class InputState;
class Entity;
class TimeTracker;

#define NW_MGR_SERVER (NetworkManagerServer::getInstance())

#define NW_MGR_SERVER_CALLBACKS NetworkManagerServer::sProcessPacket, NetworkManagerServer::sHandleNoResponse, NetworkManagerServer::sHandleConnectionReset, NetworkManagerServer::sGetClientProxy, NetworkManagerServer::sHandleClientDisconnected

typedef void (*HandleNewClientFunc)(uint8_t playerID, std::string playerName);
typedef void (*HandleLostClientFunc)(ClientProxy* clientProxy, uint8_t index);
typedef InputState* (*InputStateCreationFunc)();
typedef void (*InputStateReleaseFunc)(InputState* inputState);

class NetworkManagerServer
{
public:
    static void create(ServerHelper* serverHelper, HandleNewClientFunc handleNewClientFunc, HandleLostClientFunc handleLostClientFunc, InputStateCreationFunc inputCreationFunc, InputStateReleaseFunc inputStateReleaseFunc);
    static NetworkManagerServer* getInstance();
    static void destroy();
    
    static void sProcessPacket(InputMemoryBitStream& inputStream, MachineAddress* fromAddress);
    static void sHandleNoResponse();
    static void sHandleConnectionReset(MachineAddress* fromAddress);
    static ClientProxy* sGetClientProxy(uint8_t playerID);
    static void sHandleClientDisconnected(ClientProxy* clientProxy);
    
    void processIncomingPackets();
    void sendOutgoingPackets();
    void registerEntity(Entity* e);
    void deregisterEntity(Entity* e);
    void setStateDirty(uint32_t networkID, uint16_t dirtyState);
    ClientProxy* getClientProxy(uint8_t playerID) const;
    int getMoveCount();
    int getAverageMoveCount() const;
    int getLowestNonHostMoveCount() const;
    int getHostMoveCount() const;
    uint8_t getNumClientsConnected();
    MachineAddress* getServerAddress();
    bool isConnected();
    ServerHelper* getServerHelper();
    void setMap(uint32_t value);
    EntityManager* getEntityManager();
    
private:
    static NetworkManagerServer* s_instance;
    
    ServerHelper* _serverHelper;
    HandleNewClientFunc _handleNewClientFunc;
    HandleLostClientFunc _handleLostClientFunc;
    InputStateCreationFunc _inputStateCreationFunc;
    InputStateReleaseFunc _inputStateReleaseFunc;
    EntityManager* _entityManager;
    TimeTracker* _timeTracker;
    Pool<ReplicationManagerTransmissionData> _replicationManagerTransmissionDatas;
    std::map<size_t, ClientProxy*> _addressHashToClientMap;
    std::map<int, ClientProxy*> _playerIDToClientMap;
    uint8_t _nextPlayerID;
    uint32_t _map;
    
    void processPacket(InputMemoryBitStream& inputStream, MachineAddress* fromAddress);
    void handleNoResponse();
    void handleConnectionReset(MachineAddress* fromAddress);
    void sendPacket(const OutputMemoryBitStream& outputStream, MachineAddress* fromAddress);
    void handlePacketFromNewClient(InputMemoryBitStream& inputStream, MachineAddress* fromAddress);
    void processPacket(ClientProxy* clientProxy, InputMemoryBitStream& inputStream);
    void sendWelcomePacket(ClientProxy* clientProxy);
    void sendLocalPlayerAddedPacket(ClientProxy* clientProxy, uint8_t index);
    void sendStatePacketToClient(ClientProxy* clientProxy);
    void writeLastMoveTimestampIfDirty(OutputMemoryBitStream& outputStream, ClientProxy* clientProxy);
    void handleInputPacket(ClientProxy* clientProxy, InputMemoryBitStream& inputStream);
    void handleAddLocalPlayerPacket(ClientProxy* clientProxy, InputMemoryBitStream& inputStream);
    void handleDropLocalPlayerPacket(ClientProxy* clientProxy, InputMemoryBitStream& inputStream);
    void handleClientDisconnected(ClientProxy* clientProxy);
    void updateNextPlayerID();
    
    NetworkManagerServer(ServerHelper* serverHelper, HandleNewClientFunc handleNewClientFunc, HandleLostClientFunc handleLostClientFunc, InputStateCreationFunc inputCreationFunc, InputStateReleaseFunc inputStateReleaseFunc);
    ~NetworkManagerServer();
    NetworkManagerServer(const NetworkManagerServer&);
    NetworkManagerServer& operator=(const NetworkManagerServer&);
};
