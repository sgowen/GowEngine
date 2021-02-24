//
//  NetworkServer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "NetworkServer.hpp"

#include "ReplicationTransmissionData.hpp"
#include "InputMemoryBitStream.hpp"
#include "OutputMemoryBitStream.hpp"
#include "DeliveryNotificationManager.hpp"
#include "SocketAddress.hpp"
#include "ClientProxy.hpp"
#include "InputState.hpp"
#include "Entity.hpp"
#include "TimeTracker.hpp"
#include "PlatformMacros.hpp"
#include "StringUtil.hpp"
#include "EntityRegistry.hpp"
#include "StringUtil.hpp"
#include "GowUtil.hpp"
#include "NetworkClient.hpp"
#include "ReplicationManagerServer.hpp"
#include "InstanceRegistry.hpp"
#include "SocketAddress.hpp"
#include "EntityIDManager.hpp"
#include "SocketUtil.hpp"

#include <assert.h>

NetworkServer* NetworkServer::s_instance = NULL;

void NetworkServer::create(uint16_t port, uint8_t maxNumPlayers, OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf, HandleNewClientFunc hncf, HandleLostClientFunc hlcf, InputStateCreationFunc iscf, InputStateReleaseFunc isrf)
{
    assert(s_instance == NULL);
    
    INST_REG.get<TimeTracker>(INSK_TIME_SRVR)->reset();
    INST_REG.get<EntityIDManager>(INSK_EID_SRVR)->resetNextNetworkEntityID();
    INST_REG.get<EntityIDManager>(INSK_EID_SRVR)->resetNextPlayerEntityID();
    
    s_instance = new NetworkServer(port, maxNumPlayers, oerf, oedf, hncf, hlcf, iscf, isrf);
}

NetworkServer * NetworkServer::getInstance()
{
    return s_instance;
}

void NetworkServer::destroy()
{
    assert(s_instance != NULL);
    
    delete s_instance;
    s_instance = NULL;
}

void NetworkServer::processIncomingPackets()
{
    _packetHandler.processIncomingPackets();
    
    std::vector<ClientProxy*> clientsToDC;
    
    TimeTracker* tt = INST_REG.get<TimeTracker>(INSK_TIME_SRVR);
    uint32_t minAllowedLastPacketFromClientTime = tt->_time < NW_CLNT_TIMEOUT ? 0 : tt->_time - NW_CLNT_TIMEOUT;
    for (auto& pair: _addressHashToClientMap)
    {
        if (pair.second.getLastPacketFromClientTime() < minAllowedLastPacketFromClientTime)
        {
            ClientProxy& cp = pair.second;
            clientsToDC.push_back(&cp);
        }
    }
    
    for (ClientProxy* cp: clientsToDC)
    {
        handleClientDisconnected(*cp);
    }
}

void NetworkServer::sendOutgoingPackets()
{
    for (auto& pair: _addressHashToClientMap)
    {
        ClientProxy& cp = pair.second;
        TimeTracker* tt = INST_REG.get<TimeTracker>(INSK_TIME_SRVR);
        cp.getDeliveryNotificationManager().processTimedOutPackets(tt->_time);
        
        sendStatePacketToClient(cp);
    }
}

void NetworkServer::registerEntity(Entity* e)
{
    _entityRegistry.registerEntity(e);
}

void NetworkServer::deregisterEntity(Entity* e)
{
    _entityRegistry.deregisterEntity(e);
}

void NetworkServer::deregisterAllEntities()
{
    _entityRegistry.deregisterAll();
}

void NetworkServer::setStateDirty(uint32_t networkID, uint8_t dirtyState)
{
    assert(dirtyState > 0);
    
    for (auto& pair: _addressHashToClientMap)
    {
        pair.second.getReplicationManagerServer().setStateDirty(networkID, dirtyState);
    }
}

ClientProxy* NetworkServer::getClientProxy(uint8_t playerID) const
{
    auto it = _playerIDToClientMap.find(playerID);
    if (it != _playerIDToClientMap.end())
    {
        return it->second;
    }
    
    return NULL;
}

int NetworkServer::getMoveCount()
{
    int lowestMoveCount = 0;
    
    ClientProxy* cp = getClientProxy(1);
    if (cp != NULL)
    {
        lowestMoveCount = cp->getUnprocessedMoveList().getMoveCount();
    }
    
    for (auto& pair : _playerIDToClientMap)
    {
        int moveCount = pair.second->getUnprocessedMoveList().getMoveCount();
        if (moveCount < lowestMoveCount)
        {
            lowestMoveCount = moveCount;
        }
    }
    
    uint32_t expectedMoveIndex = getNumMovesProcessed();
    int validMoveCount = 0;
    for (int i = 0; i < lowestMoveCount; ++i)
    {
        bool isMoveValid = true;
        for (auto& pair : _playerIDToClientMap)
        {
            ClientProxy* cp = pair.second;
            assert(cp != NULL);

            MoveList& ml = cp->getUnprocessedMoveList();
            Move* m = ml.getMoveAtIndex(i);
            assert(m != NULL);

            if (expectedMoveIndex != m->getIndex())
            {
                isMoveValid = false;
                break;
            }
        }

        if (isMoveValid)
        {
            ++validMoveCount;
            ++expectedMoveIndex;
        }
    }
    
    assert(lowestMoveCount == validMoveCount);
    
    return validMoveCount;
}

uint8_t NetworkServer::getNumClientsConnected()
{
    return static_cast<uint8_t>(_addressHashToClientMap.size());
}

uint8_t NetworkServer::getNumPlayersConnected()
{
    return static_cast<uint8_t>(_playerIDToClientMap.size());
}

SocketAddress& NetworkServer::getServerAddress()
{
    return _packetHandler.getSocketAddress();
}

bool NetworkServer::connect()
{
    if (SOCKET_UTIL.isLoggingEnabled())
    {
        LOG("Server Initializing PacketHandler at port %hu", _port);
    }
    
    int error = _packetHandler.connect();
    if (error != NO_ERROR &&
        SOCKET_UTIL.isLoggingEnabled())
    {
        LOG("Server connect failed. Error code %d", error);
    }
    
    return error == NO_ERROR;
}

void NetworkServer::onEntityRegistered(Entity* e)
{
    for (auto& pair: _addressHashToClientMap)
    {
        pair.second.getReplicationManagerServer().replicateCreate(e->getID(), ALL_DIRTY_STATE);
    }
    
    _onEntityRegisteredFunc(e);
}

void NetworkServer::onEntityDeregistered(Entity* e)
{
    for (auto& pair: _addressHashToClientMap)
    {
        pair.second.getReplicationManagerServer().replicateDestroy(e->getID());
    }
    
    _onEntityDeregisteredFunc(e);
}

EntityRegistry& NetworkServer::getEntityRegistry()
{
    return _entityRegistry;
}

void NetworkServer::processPacket(InputMemoryBitStream& imbs, SocketAddress* fromAddress)
{
    if (SOCKET_UTIL.isLoggingEnabled())
    {
        LOG("Server processPacket bit length: %d", imbs.getRemainingBitCount());
    }
    
    auto it = _addressHashToClientMap.find(fromAddress->getHash());
    if (it == _addressHashToClientMap.end())
    {
        if (_playerIDToClientMap.size() < _maxNumPlayers)
        {
            if (SOCKET_UTIL.isLoggingEnabled())
            {
                LOG("Server is processing new client from %s", fromAddress->toString().c_str());
            }
            
            handlePacketFromNewClient(imbs, fromAddress);
        }
        else
        {
            if (SOCKET_UTIL.isLoggingEnabled())
            {
                LOG("Server is at max capacity, blocking new client...");
            }
        }
    }
    else
    {
        processPacket((*it).second, imbs);
    }
}

void NetworkServer::onMovesProcessed(uint8_t moveCount)
{
    _numMovesProcessed += moveCount;
}

uint32_t NetworkServer::getNumMovesProcessed()
{
    return _numMovesProcessed;
}

const std::map<int, ClientProxy*>& NetworkServer::playerIDToClientMap()
{
    return _playerIDToClientMap;
}

void NetworkServer::sendPacket(const OutputMemoryBitStream& ombs, SocketAddress* fromAddress)
{
    if (SOCKET_UTIL.isLoggingEnabled())
    {
        LOG("Server    sendPacket bit length: %d", ombs.getBitLength());
    }
    
    _packetHandler.sendPacket(ombs, fromAddress);
}

void NetworkServer::handlePacketFromNewClient(InputMemoryBitStream& imbs, SocketAddress* fromAddress)
{
    // read the beginning- is it a hello?
    uint8_t packetType;
    imbs.read<uint8_t, 4>(packetType);
    if (packetType == NWPT_HELLO)
    {
        // read the name
        std::string name;
        imbs.readSmall(name);
        
        _addressHashToClientMap.emplace(std::piecewise_construct,
                  std::forward_as_tuple(fromAddress->getHash()),
                  std::forward_as_tuple(_entityRegistry, fromAddress, name, _nextPlayerID));
        ClientProxy& cp = _addressHashToClientMap.at(fromAddress->getHash());
        std::string username = cp.getUsername();
        uint8_t playerID = cp.getPlayerID();
        _playerIDToClientMap[playerID] = &cp;
        
        // tell the server about this client
        _handleNewClientFunc(username, playerID);
        
        //and welcome the client...
        sendWelcomePacket(cp);
        
        // and now init the replication manager with everything we know about!
        for (auto& pair: _entityRegistry.getMap())
        {
            cp.getReplicationManagerServer().replicateCreate(pair.first, ALL_DIRTY_STATE);
        }
        
        resetNextPlayerID();
    }
    else
    {
        if (SOCKET_UTIL.isLoggingEnabled())
        {
            LOG("Unknown packet type received from %s", fromAddress->toString().c_str());
        }
    }
}

void NetworkServer::processPacket(ClientProxy& cp, InputMemoryBitStream& imbs)
{
    cp.updateLastPacketTime();
    
    uint8_t packetType;
    imbs.read<uint8_t, 4>(packetType);
    
    switch (packetType)
    {
        case NWPT_HELLO:
            //need to resend welcome. to be extra safe we should check the name is the one we expect from this address,
            //otherwise something weird is going on...
            sendWelcomePacket(cp);
            break;
        case NWPT_INPUT:
            handleInputPacket(cp, imbs);
            break;
        case NWPT_ADD_LOCAL_PLAYER:
            handleAddLocalPlayerPacket(cp, imbs);
            break;
        case NWPT_DROP_LOCAL_PLAYER:
            handleDropLocalPlayerPacket(cp, imbs);
            break;
        case NWPT_CLNT_EXIT:
            handleClientDisconnected(cp);
            break;
        default:
            if (SOCKET_UTIL.isLoggingEnabled())
            {
                LOG("Unknown packet type received from %s", cp.getSocketAddress()->toString().c_str());
            }
            break;
    }
}

void NetworkServer::sendWelcomePacket(ClientProxy& cp)
{
    OutputMemoryBitStream ombs;
    ombs.write<uint8_t, 4>(static_cast<uint8_t>(NWPT_WELCOME));
    ombs.write<uint8_t, 3>(cp.getPlayerID());
    sendPacket(ombs, cp.getSocketAddress());
    
    if (SOCKET_UTIL.isLoggingEnabled())
    {
        LOG("Server welcoming new client '%s' as player %d", cp.getUsername().c_str(), cp.getPlayerID());
    }
}

void NetworkServer::sendStatePacketToClient(ClientProxy& cp)
{
    OutputMemoryBitStream ombs;
    ombs.write<uint8_t, 4>(static_cast<uint8_t>(NWPT_STATE));
    
    ombs.write(_numMovesProcessed);
    
    InFlightPacket* ifp = cp.getDeliveryNotificationManager().writeState(ombs);
    
    bool isTimestampDirty = cp.isLastMoveTimestampDirty();
    ombs.write(isTimestampDirty);
    if (isTimestampDirty)
    {
        uint32_t lastProcessedMoveTimestamp = cp.getUnprocessedMoveList().getLastProcessedMoveTimestamp();
        ombs.write(lastProcessedMoveTimestamp);
        cp.setLastMoveTimestampDirty(false);
    }
    
    ReplicationTransmissionData* rtd = _poolRMTD.obtain();
    rtd->reset(&cp.getReplicationManagerServer(), &_entityRegistry, &_poolRMTD);
    
    cp.getReplicationManagerServer().write(ombs, rtd);
    
    TransmissionData* td = ifp->getTransmissionData('RPLM');
    if (td != NULL)
    {
        td->free();
    }
    
    ifp->setTransmissionData('RPLM', rtd);
    
    sendPacket(ombs, cp.getSocketAddress());
}

void NetworkServer::handleInputPacket(ClientProxy& cp, InputMemoryBitStream& imbs)
{
    if (!cp.getDeliveryNotificationManager().readAndProcessState(imbs))
    {
        return;
    }
    
    bool hasMoves;
    imbs.read(hasMoves);
    if (!hasMoves)
    {
        return;
    }
    
    uint8_t moveCount = 0;
    imbs.read<uint8_t, 4>(moveCount);
    
	InputState* referenceInputState = NULL;
	bool isRefInputStateOrphaned = false;
    
    for (; moveCount > 0; --moveCount)
    {
        Move move = Move(_inputStateCreationFunc());
        
        bool isCopy;
        imbs.read(isCopy);
        if (isCopy)
        {
            assert(referenceInputState != NULL);
            
            uint32_t timeStamp;
            imbs.read(timeStamp);
            move.setTimestamp(timeStamp);
            uint32_t index;
            imbs.read(index);
            move.setIndex(index);
            move.copyInputState(referenceInputState);
        }
        else
        {
            move.read(imbs);
        }

		if (isRefInputStateOrphaned)
		{
            _inputStateReleaseFunc(referenceInputState);
		}

		referenceInputState = move.inputState();
        isRefInputStateOrphaned = !cp.getUnprocessedMoveList().addMoveIfNew(move);
    }
    
    if (isRefInputStateOrphaned)
    {
        _inputStateReleaseFunc(referenceInputState);
    }
}

void NetworkServer::handleAddLocalPlayerPacket(ClientProxy& cp, InputMemoryBitStream& imbs)
{
    if (_playerIDToClientMap.size() < _maxNumPlayers)
    {
        // read the current number of local players for this client at the time when the request was made
        uint8_t requestedIndex;
        imbs.read(requestedIndex);
        
        uint8_t playerID = cp.getPlayerID(requestedIndex);
        if (playerID == NW_INPUT_UNASSIGNED)
        {
            std::string localUsername = StringUtil::format("%s(%d)", cp.getUsername().c_str(), requestedIndex);
            uint8_t playerID = _nextPlayerID;
            
            cp.onLocalPlayerAdded(playerID);
            
            _playerIDToClientMap[playerID] = &cp;
            
            // tell the server about this client
            _handleNewClientFunc(localUsername, playerID);
            
            resetNextPlayerID();
        }
        
        // and welcome the new local player...
        sendLocalPlayerAddedPacket(cp);
    }
    else
    {
        OutputMemoryBitStream ombs;
        ombs.write<uint8_t, 4>(static_cast<uint8_t>(NWPT_LOCAL_PLAYER_DENIED));
        sendPacket(ombs, cp.getSocketAddress());
    }
}

void NetworkServer::sendLocalPlayerAddedPacket(ClientProxy& cp)
{
    uint8_t index = cp.getNumPlayers() - 1;
    uint8_t playerID = cp.getPlayerID(index);
    
    OutputMemoryBitStream ombs;
    ombs.write<uint8_t, 4>(static_cast<uint8_t>(NWPT_LOCAL_PLAYER_ADDED));
    ombs.write<uint8_t, 3>(playerID);
    sendPacket(ombs, cp.getSocketAddress());
    
    std::string localPlayerName = StringUtil::format("%s(%d)", cp.getUsername().c_str(), index);
    if (SOCKET_UTIL.isLoggingEnabled())
    {
        LOG("Server welcoming new client local player '%s' as player %d", localPlayerName.c_str(), playerID);
    }
}

void NetworkServer::handleDropLocalPlayerPacket(ClientProxy& cp, InputMemoryBitStream& imbs)
{
    // read the index to drop
    uint8_t localPlayerIndex;
    imbs.read(localPlayerIndex);
    
    // If the primary player on this client wants to drop, a disconnect request should be fired off instead of a drop
    assert(localPlayerIndex >= 1);
    
    uint8_t playerID = cp.getPlayerID(localPlayerIndex);
    if (playerID == NW_INPUT_UNASSIGNED)
    {
        return;
    }
    
    _playerIDToClientMap.erase(playerID);
    
    _handleLostClientFunc(cp, localPlayerIndex);
    
    cp.onLocalPlayerRemoved(playerID);
    
    resetNextPlayerID();
}

void NetworkServer::handleClientDisconnected(ClientProxy& cp)
{
    if (SOCKET_UTIL.isLoggingEnabled())
    {
        LOG("Client is leaving the server");
    }
    
    for (uint8_t i = 0; i < cp.getNumPlayers(); ++i)
    {
        _playerIDToClientMap.erase(cp.getPlayerID(i));
    }
    
    _handleLostClientFunc(cp, 0);
    
    _addressHashToClientMap.erase(cp.getSocketAddress()->getHash());
    
    resetNextPlayerID();
    
    if (getNumClientsConnected() == 0)
    {
        deregisterAllEntities();
    }
}

void NetworkServer::resetNextPlayerID()
{
    _nextPlayerID = 1;
    for (auto& pair : _playerIDToClientMap)
    {
        if (pair.first == _nextPlayerID)
        {
            ++_nextPlayerID;
        }
    }
}

void cb_nw_srvr_processPacket(InputMemoryBitStream& imbs, SocketAddress* fromAddress)
{
    NW_SRVR->processPacket(imbs, fromAddress);
}

void cb_nw_srvr_onEntityRegistered(Entity* e)
{
    NW_SRVR->onEntityRegistered(e);
}

void cb_nw_srvr_onEntityDeregistered(Entity* e)
{
    NW_SRVR->onEntityDeregistered(e);
}

NetworkServer::NetworkServer(uint16_t port, uint8_t maxNumPlayers, OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf, HandleNewClientFunc hncf, HandleLostClientFunc hlcf, InputStateCreationFunc iscf, InputStateReleaseFunc isrf) :
_packetHandler(INST_REG.get<TimeTracker>(INSK_TIME_SRVR), port, cb_nw_srvr_processPacket),
_onEntityRegisteredFunc(oerf),
_onEntityDeregisteredFunc(oedf),
_handleNewClientFunc(hncf),
_handleLostClientFunc(hlcf),
_inputStateCreationFunc(iscf),
_inputStateReleaseFunc(isrf),
_entityRegistry(cb_nw_srvr_onEntityRegistered, cb_nw_srvr_onEntityDeregistered),
_nextPlayerID(1),
_maxNumPlayers(maxNumPlayers),
_numMovesProcessed(0),
_port(port)
{
    // Empty
}

NetworkServer::~NetworkServer()
{
    for (auto& pair : _addressHashToClientMap)
    {
        ClientProxy& cp = pair.second;
        
        OutputMemoryBitStream ombs;
        ombs.write<uint8_t, 4>(static_cast<uint8_t>(NWPT_SRVR_EXIT));
        sendPacket(ombs, cp.getSocketAddress());
    }
    
    _addressHashToClientMap.clear();
    _playerIDToClientMap.clear();
    
    deregisterAllEntities();
}
