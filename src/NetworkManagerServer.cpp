//
//  NetworkManagerServer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "NetworkManagerServer.hpp"

#include "ReplicationManagerTransmissionData.hpp"
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
#include "NetworkManagerClient.hpp"
#include "ReplicationManagerServer.hpp"
#include "InstanceRegistry.hpp"
#include "SocketAddress.hpp"
#include "EntityIDManager.hpp"

#include <assert.h>

NetworkManagerServer* NetworkManagerServer::s_instance = NULL;

void NetworkManagerServer::create(uint16_t port, uint8_t maxNumPlayers, OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf, HandleNewClientFunc hncf, HandleLostClientFunc hlcf, InputStateCreationFunc iscf, InputStateReleaseFunc isrf)
{
    assert(s_instance == NULL);
    
    INST_REG.get<TimeTracker>(INSK_TIME_SRVR)->reset();
    INST_REG.get<EntityIDManager>(INSK_EID_SRVR)->resetNextNetworkEntityID();
    INST_REG.get<EntityIDManager>(INSK_EID_SRVR)->resetNextPlayerEntityID();
    
    s_instance = new NetworkManagerServer(port, maxNumPlayers, oerf, oedf, hncf, hlcf, iscf, isrf);
}

NetworkManagerServer * NetworkManagerServer::getInstance()
{
    return s_instance;
}

void NetworkManagerServer::destroy()
{
    assert(s_instance != NULL);
    
    delete s_instance;
    s_instance = NULL;
}

void NetworkManagerServer::processIncomingPackets()
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

void NetworkManagerServer::sendOutgoingPackets()
{
    for (auto& pair: _addressHashToClientMap)
    {
        ClientProxy& cp = pair.second;
        TimeTracker* tt = INST_REG.get<TimeTracker>(INSK_TIME_SRVR);
        cp.getDeliveryNotificationManager().processTimedOutPackets(tt->_time);
        
        if (cp.isLastMoveTimestampDirty())
        {
            sendStatePacketToClient(cp);
        }
    }
}

void NetworkManagerServer::registerEntity(Entity* e)
{
    for (auto& pair: _addressHashToClientMap)
    {
        pair.second.getReplicationManagerServer().replicateCreate(e->getID(), ALL_DIRTY_STATE);
    }
    
    _entityRegistry.registerEntity(e);
}

void NetworkManagerServer::deregisterEntity(Entity* e)
{
    for (auto& pair: _addressHashToClientMap)
    {
        pair.second.getReplicationManagerServer().replicateDestroy(e->getID());
    }
    
    _entityRegistry.deregisterEntity(e);
}

void NetworkManagerServer::setStateDirty(uint32_t networkID, uint8_t dirtyState)
{
    assert(dirtyState > 0);
    
    for (auto& pair: _addressHashToClientMap)
    {
        pair.second.getReplicationManagerServer().setStateDirty(networkID, dirtyState);
    }
}

ClientProxy* NetworkManagerServer::getClientProxy(uint8_t playerID) const
{
    auto it = _playerIDToClientMap.find(playerID);
    if (it != _playerIDToClientMap.end())
    {
        return it->second;
    }
    
    return NULL;
}

int NetworkManagerServer::getHostMoveCount()
{
    int ret = 0;
    
    ClientProxy* cp = getClientProxy(1);
    if (cp != NULL)
    {
        ret = cp->getUnprocessedMoveList().getMoveCount();
    }
    
    return ret;
}

uint8_t NetworkManagerServer::getNumClientsConnected()
{
    return static_cast<uint8_t>(_addressHashToClientMap.size());
}

uint8_t NetworkManagerServer::getNumPlayersConnected()
{
    return static_cast<uint8_t>(_playerIDToClientMap.size());
}

SocketAddress& NetworkManagerServer::getServerAddress()
{
    return _packetHandler.getSocketAddress();
}

bool NetworkManagerServer::isConnected()
{
    return _packetHandler.isConnected();
}

EntityRegistry& NetworkManagerServer::getEntityRegistry()
{
    return _entityRegistry;
}

void NetworkManagerServer::processPacket(InputMemoryBitStream& imbs, SocketAddress* fromAddress)
{
#if IS_DEBUG
    LOG("Server processPacket bit length: %d", imbs.getRemainingBitCount());
#endif
    
    auto it = _addressHashToClientMap.find(fromAddress->getHash());
    if (it == _addressHashToClientMap.end())
    {
        if (_playerIDToClientMap.size() < _maxNumPlayers)
        {
            LOG("New Client with %s", fromAddress->toString().c_str());
            
            handlePacketFromNewClient(imbs, fromAddress);
        }
        else
        {
            LOG("Server is at max capacity, blocking new client...");
        }
    }
    else
    {
        processPacket((*it).second, imbs);
    }
}

void NetworkManagerServer::sendPacket(const OutputMemoryBitStream& ombs, SocketAddress* fromAddress)
{
#if IS_DEBUG
    LOG("Server outgoing packet bit length: %d", ombs.getBitLength());
#endif
    
    _packetHandler.sendPacket(ombs, fromAddress);
}

void NetworkManagerServer::handlePacketFromNewClient(InputMemoryBitStream& imbs, SocketAddress* fromAddress)
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
        LOG("Unknown packet type received from %s", fromAddress->toString().c_str());
    }
}

void NetworkManagerServer::processPacket(ClientProxy& cp, InputMemoryBitStream& imbs)
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
            if (cp.getDeliveryNotificationManager().readAndProcessState(imbs))
            {
                handleInputPacket(cp, imbs);
            }
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
            LOG("Unknown packet type received from %s", cp.getSocketAddress()->toString().c_str());
            break;
    }
}

void NetworkManagerServer::sendWelcomePacket(ClientProxy& cp)
{
    OutputMemoryBitStream ombs;
    ombs.write<uint8_t, 4>(static_cast<uint8_t>(NWPT_WELCOME));
    ombs.write<uint8_t, 3>(cp.getPlayerID());
    sendPacket(ombs, cp.getSocketAddress());
    
    LOG("Server welcoming new client '%s' as player %d", cp.getUsername().c_str(), cp.getPlayerID());
}

void NetworkManagerServer::sendStatePacketToClient(ClientProxy& cp)
{
    OutputMemoryBitStream ombs;
    ombs.write<uint8_t, 4>(static_cast<uint8_t>(NWPT_STATE));
    
    InFlightPacket* ifp = cp.getDeliveryNotificationManager().writeState(ombs);
    
    writeLastMoveTimestampIfDirty(ombs, cp);
    
    ReplicationManagerTransmissionData* rmtd = _poolRMTD.obtain();
    rmtd->reset(&cp.getReplicationManagerServer(), &_entityRegistry, &_poolRMTD);
    
    cp.getReplicationManagerServer().write(ombs, rmtd);
    
    TransmissionData* td = ifp->getTransmissionData('RPLM');
    if (td != NULL)
    {
        td->free();
    }
    
    ifp->setTransmissionData('RPLM', rmtd);
    
    sendPacket(ombs, cp.getSocketAddress());
}

void NetworkManagerServer::writeLastMoveTimestampIfDirty(OutputMemoryBitStream& ombs, ClientProxy& cp)
{
    bool isTimestampDirty = cp.isLastMoveTimestampDirty();
    ombs.write(isTimestampDirty);
    if (isTimestampDirty)
    {
        uint32_t lastProcessedMoveTimestamp = cp.getUnprocessedMoveList().getLastProcessedMoveTimestamp();
        
        ombs.write(lastProcessedMoveTimestamp);
        
        cp.setLastMoveTimestampDirty(false);
    }
}

void NetworkManagerServer::handleInputPacket(ClientProxy& cp, InputMemoryBitStream& imbs)
{
    uint8_t moveCount = 0;
    imbs.read<uint8_t, 2>(moveCount);
    
	InputState* referenceInputState = NULL;
	bool isRefInputStateOrphaned = false;
    
    for (; moveCount > 0; --moveCount)
    {
        Move move = Move(_inputStateCreationFunc());
        
        bool isCopy;
        imbs.read(isCopy);
        if (isCopy)
        {
            if (referenceInputState == NULL)
            {
                LOG("Unexpected Network State!");
                
                return;
            }
            
            uint32_t timeStamp;
            imbs.read(timeStamp);
            move.setTimestamp(timeStamp);
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

void NetworkManagerServer::handleAddLocalPlayerPacket(ClientProxy& cp, InputMemoryBitStream& imbs)
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

void NetworkManagerServer::sendLocalPlayerAddedPacket(ClientProxy& cp)
{
    uint8_t index = cp.getNumPlayers() - 1;
    uint8_t playerID = cp.getPlayerID(index);
    
    OutputMemoryBitStream ombs;
    ombs.write<uint8_t, 4>(static_cast<uint8_t>(NWPT_LOCAL_PLAYER_ADDED));
    ombs.write<uint8_t, 3>(playerID);
    sendPacket(ombs, cp.getSocketAddress());
    
    std::string localPlayerName = StringUtil::format("%s(%d)", cp.getUsername().c_str(), index);
    LOG("Server welcoming new client local player '%s' as player %d", localPlayerName.c_str(), playerID);
}

void NetworkManagerServer::handleDropLocalPlayerPacket(ClientProxy& cp, InputMemoryBitStream& imbs)
{
    // read the index to drop
    uint8_t index;
    imbs.read(index);
    
    // If the primary player on this client wants to drop, a disconnect request should be fired off instead of a drop
    assert(index >= 1);
    
    uint8_t playerID = cp.getPlayerID(index);
    if (playerID == NW_INPUT_UNASSIGNED)
    {
        return;
    }
    
    _playerIDToClientMap.erase(playerID);
    
    _handleLostClientFunc(cp, index);
    
    cp.onLocalPlayerRemoved(playerID);
    
    resetNextPlayerID();
}

void NetworkManagerServer::handleClientDisconnected(ClientProxy& cp)
{
    LOG("Client is leaving the server");
    
    for (uint8_t i = 0; i < cp.getNumPlayers(); ++i)
    {
        _playerIDToClientMap.erase(cp.getPlayerID(i));
    }
    
    _addressHashToClientMap.erase(cp.getSocketAddress()->getHash());
    
    _handleLostClientFunc(cp, 0);
    
    resetNextPlayerID();
}

void NetworkManagerServer::resetNextPlayerID()
{
    _nextPlayerID = 1;
    for (auto& pair : _playerIDToClientMap)
    {
        if (pair.first == _nextPlayerID)
        {
            ++_nextPlayerID;
        }
    }
    
    LOG("_nextPlayerID: %d", _nextPlayerID);
}

void cb_server_processPacket(InputMemoryBitStream& imbs, SocketAddress* fromAddress)
{
    NW_MGR_SRVR->processPacket(imbs, fromAddress);
}

NetworkManagerServer::NetworkManagerServer(uint16_t port, uint8_t maxNumPlayers, OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf, HandleNewClientFunc hncf, HandleLostClientFunc hlcf, InputStateCreationFunc iscf, InputStateReleaseFunc isrf) :
_packetHandler(INST_REG.get<TimeTracker>(INSK_TIME_SRVR), port, cb_server_processPacket),
_handleNewClientFunc(hncf),
_handleLostClientFunc(hlcf),
_inputStateCreationFunc(iscf),
_inputStateReleaseFunc(isrf),
_entityRegistry(oerf, oedf),
_nextPlayerID(1),
_maxNumPlayers(maxNumPlayers)
{
    // Empty
}

NetworkManagerServer::~NetworkManagerServer()
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
}
