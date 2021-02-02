//
//  NetworkManagerServer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "NetworkManagerServer.hpp"

#include "EntityManager.hpp"
#include "ReplicationManagerTransmissionData.hpp"
#include "ServerHelper.hpp"
#include "InputMemoryBitStream.hpp"
#include "OutputMemoryBitStream.hpp"
#include "DeliveryNotificationManager.hpp"
#include "MachineAddress.hpp"
#include "ClientProxy.hpp"
#include "InputState.hpp"
#include "Entity.hpp"
#include "TimeTracker.hpp"
#include "PlatformMacros.hpp"

#include "StringUtil.hpp"
#include "EntityManager.hpp"
#include "StringUtil.hpp"
#include "GowUtil.hpp"
#include "NetworkManagerClient.hpp"
#include "ReplicationManagerServer.hpp"
#include "InstanceManager.hpp"

#include <assert.h>

NetworkManagerServer* NetworkManagerServer::s_instance = NULL;

void NetworkManagerServer::create(ServerHelper* serverHelper, HandleNewClientFunc handleNewClientFunc, HandleLostClientFunc handleLostClientFunc, InputStateCreationFunc inputStateCreationFunc, InputStateReleaseFunc inputStateReleaseFunc)
{
    assert(!s_instance);
    
    s_instance = new NetworkManagerServer(serverHelper, handleNewClientFunc, handleLostClientFunc, inputStateCreationFunc, inputStateReleaseFunc);
}

NetworkManagerServer * NetworkManagerServer::getInstance()
{
    return s_instance;
}

void NetworkManagerServer::destroy()
{
    assert(s_instance);
    
    delete s_instance;
    s_instance = NULL;
}

void NetworkManagerServer::sProcessPacket(InputMemoryBitStream& inputStream, MachineAddress* fromAddress)
{
    NW_MGR_SERVER->processPacket(inputStream, fromAddress);
}

void NetworkManagerServer::sHandleNoResponse()
{
    NW_MGR_SERVER->handleNoResponse();
}

void NetworkManagerServer::sHandleConnectionReset(MachineAddress* fromAddress)
{
    NW_MGR_SERVER->handleConnectionReset(fromAddress);
}

ClientProxy* NetworkManagerServer::sGetClientProxy(uint8_t playerIndex)
{
    return NW_MGR_SERVER->getClientProxy(playerIndex + 1);
}

void NetworkManagerServer::sHandleClientDisconnected(ClientProxy* clientProxy)
{
    NW_MGR_SERVER->handleClientDisconnected(clientProxy);
}

void NetworkManagerServer::processIncomingPackets()
{
    _serverHelper->processIncomingPackets();
    
    /// Check for disconnects
    std::vector<ClientProxy*> clientsToDC;
    
    float minAllowedLastPacketFromClientTime = _timeTracker->_time - NW_CLIENT_TIMEOUT;
    for (const auto& pair: _addressHashToClientMap)
    {
        if (pair.second->getLastPacketFromClientTime() < minAllowedLastPacketFromClientTime)
        {
            // can't remove from map while in iterator, so just remember for later...
            clientsToDC.push_back(pair.second);
        }
    }
    
    for (ClientProxy* client: clientsToDC)
    {
        handleClientDisconnected(client);
    }
}

void NetworkManagerServer::sendOutgoingPackets()
{
    //let's send a client a state packet whenever their move has come in...
    for (auto it = _addressHashToClientMap.begin(), end = _addressHashToClientMap.end(); it != end; ++it)
    {
        ClientProxy* clientProxy = it->second;
        //process any timed out packets while we're going through the list
        clientProxy->getDeliveryNotificationManager().processTimedOutPackets(_timeTracker->_time);
        
        if (clientProxy->isLastMoveTimestampDirty())
        {
            sendStatePacketToClient(clientProxy);
        }
    }
}

void NetworkManagerServer::registerEntity(Entity* e)
{
    //add mapping from network id to game object
    _entityManager->registerEntity(e);
    
    //tell all client proxies this is new...
    for (const auto& pair: _addressHashToClientMap)
    {
        pair.second->getReplicationManagerServer()->replicateCreate(e->getID(), ALL_DIRTY_STATE);
    }
}

void NetworkManagerServer::deregisterEntity(Entity* e)
{
    uint32_t networkID = e->getID();
    
    _entityManager->deregisterEntity(e);
    
    //tell all client proxies to STOP replicating!
    //tell all client proxies this is new...
    for (const auto& pair: _addressHashToClientMap)
    {
        pair.second->getReplicationManagerServer()->replicateDestroy(networkID);
    }
}

void NetworkManagerServer::setStateDirty(uint32_t networkID, uint16_t dirtyState)
{
    assert(dirtyState > 0);
    
    //tell everybody this is dirty
    for (const auto& pair: _addressHashToClientMap)
    {
        pair.second->getReplicationManagerServer()->setStateDirty(networkID, dirtyState);
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

int NetworkManagerServer::getMoveCount()
{
    int ret = 0;
    
    int lowestNonHostMoveCount = getLowestNonHostMoveCount();
    int hostMoveCount = getHostMoveCount();
    
    if (lowestNonHostMoveCount == -1
        || (hostMoveCount <= lowestNonHostMoveCount
            && (hostMoveCount * 2) >= lowestNonHostMoveCount))
    {
        ret = hostMoveCount;
    }
    else if (lowestNonHostMoveCount <= hostMoveCount
             && (lowestNonHostMoveCount * 2) >= hostMoveCount)
    {
        ret = lowestNonHostMoveCount;
    }
    else if (lowestNonHostMoveCount >= 8 || hostMoveCount >= 8)
    {
        ret = getAverageMoveCount();
        
#if IS_DEBUG
        LOG("lowestNonHostMoveCount: %d, hostMoveCount: %d, finalMoveCount(avg): %d", lowestNonHostMoveCount, hostMoveCount, ret);
#endif
    }
    
    return ret;
}

int NetworkManagerServer::getAverageMoveCount() const
{
    int ret = 0;
    
    if (_addressHashToClientMap.size() > 0)
    {
        for (auto const& entry : _addressHashToClientMap)
        {
            ret += entry.second->getUnprocessedMoveList().getMoveCount();
        }
        
        ret /= _addressHashToClientMap.size();
    }
    
    return ret;
}

int NetworkManagerServer::getLowestNonHostMoveCount() const
{
    int ret = -1;
    
    if (_addressHashToClientMap.size() > 0)
    {
        for (auto const& entry : _addressHashToClientMap)
        {
            ClientProxy* client = entry.second;
            if (client->getPlayerID() != 1)
            {
                uint32_t moveCount = client->getUnprocessedMoveList().getMoveCount();
                if (moveCount < ret || ret == -1)
                {
                    ret = moveCount;
                }
            }
        }
    }
    
    return ret;
}

int NetworkManagerServer::getHostMoveCount() const
{
    int ret = 0;
    
    ClientProxy* client = getClientProxy(1);
    if (client)
    {
        ret = client->getUnprocessedMoveList().getMoveCount();
    }
    
    return ret;
}

uint8_t NetworkManagerServer::getNumClientsConnected()
{
    return static_cast<uint8_t>(_addressHashToClientMap.size());
}

MachineAddress* NetworkManagerServer::getServerAddress()
{
    return _serverHelper->getServerAddress();
}

bool NetworkManagerServer::isConnected()
{
    return _serverHelper->isConnected();
}

ServerHelper* NetworkManagerServer::getServerHelper()
{
    return _serverHelper;
}

void NetworkManagerServer::setMap(uint32_t value)
{
    _map = value;
}

EntityManager* NetworkManagerServer::getEntityManager()
{
    return _entityManager;
}

void NetworkManagerServer::processPacket(InputMemoryBitStream& inputStream, MachineAddress* fromAddress)
{
    //try to get the client proxy for this address
    //pass this to the client proxy to process
    auto it = _addressHashToClientMap.find(fromAddress->getHash());
    if (it == _addressHashToClientMap.end())
    {
        if (_playerIDToClientMap.size() < _serverHelper->maxNumPlayers())
        {
            LOG("New Client with %s", fromAddress->toString().c_str());
            
            handlePacketFromNewClient(inputStream, fromAddress);
        }
        else
        {
            LOG("Server is at max capacity, blocking new client...");
        }
    }
    else
    {
        processPacket((*it).second, inputStream);
    }
}

void NetworkManagerServer::handleNoResponse()
{
    // Unused
}

void NetworkManagerServer::handleConnectionReset(MachineAddress* fromAddress)
{
    //just dc the client right away...
    auto it = _addressHashToClientMap.find(fromAddress->getHash());
    if (it != _addressHashToClientMap.end())
    {
        handleClientDisconnected(it->second);
    }
}

void NetworkManagerServer::sendPacket(const OutputMemoryBitStream& outputStream, MachineAddress* fromAddress)
{
    _serverHelper->sendPacket(outputStream, fromAddress);
}

void NetworkManagerServer::handlePacketFromNewClient(InputMemoryBitStream& inputStream, MachineAddress* fromAddress)
{
    // read the beginning- is it a hello?
    uint8_t packetType;
    inputStream.read(packetType);
    if (packetType == NWPT_HELLO)
    {
        // read the name
        std::string name;
        inputStream.readSmall(name);
        
        if (_addressHashToClientMap.size() == 0)
        {
            if (NW_MGR_CLIENT->getPlayerName().compare(name) != 0)
            {
                // The server host MUST be the first client to join
                // TODO, this doesn't apply for dedicated servers, which I aim to support eventually
                return;
            }
        }
        
        ClientProxy* cp = new ClientProxy(_entityManager, fromAddress, name, _nextPlayerID);
        _addressHashToClientMap[fromAddress->getHash()] = cp;
        _playerIDToClientMap[cp->getPlayerID()] = cp;
        
        uint8_t playerID = cp->getPlayerID();
        std::string playerName = cp->getName();
        
        // tell the server about this client
        _handleNewClientFunc(playerID, playerName);
        
        //and welcome the client...
        sendWelcomePacket(cp);
        
        // and now init the replication manager with everything we know about!
        for (const auto& pair: _entityManager->getMap())
        {
            cp->getReplicationManagerServer()->replicateCreate(pair.first, ALL_DIRTY_STATE);
        }
        
        updateNextPlayerID();
    }
    else
    {
        _serverHelper->processSpecialPacket(packetType, inputStream, fromAddress);
    }
}

void NetworkManagerServer::processPacket(ClientProxy* clientProxy, InputMemoryBitStream& inputStream)
{
    //remember we got a packet so we know not to disconnect for a bit
    clientProxy->updateLastPacketTime();
    
    uint8_t packetType;
    inputStream.read(packetType);
    
    switch (packetType)
    {
        case NWPT_HELLO:
            //need to resend welcome. to be extra safe we should check the name is the one we expect from this address,
            //otherwise something weird is going on...
            sendWelcomePacket(clientProxy);
            break;
        case NWPT_INPUT:
            if (clientProxy->getDeliveryNotificationManager().readAndProcessState(inputStream))
            {
                handleInputPacket(clientProxy, inputStream);
            }
            break;
        case NWPT_ADD_LOCAL_PLAYER:
            handleAddLocalPlayerPacket(clientProxy, inputStream);
            break;
        case NWPT_DROP_LOCAL_PLAYER:
            handleDropLocalPlayerPacket(clientProxy, inputStream);
            break;
        default:
            _serverHelper->processSpecialPacket(packetType, inputStream, clientProxy->getMachineAddress());
            break;
    }
}

void NetworkManagerServer::sendWelcomePacket(ClientProxy* clientProxy)
{
    OutputMemoryBitStream packet;
    
    packet.write(static_cast<uint8_t>(NWPT_WELCOME));
    packet.write<uint8_t, 3>(clientProxy->getPlayerID());
    
    LOG("Server welcoming new client '%s' as player %d", clientProxy->getName().c_str(), clientProxy->getPlayerID());
    
    sendPacket(packet, clientProxy->getMachineAddress());
}

void NetworkManagerServer::sendStatePacketToClient(ClientProxy* clientProxy)
{
    //build state packet
    OutputMemoryBitStream statePacket;
    
    //it's state!
    statePacket.write(static_cast<uint8_t>(NWPT_STATE));
    
    InFlightPacket* ifp = clientProxy->getDeliveryNotificationManager().writeState(statePacket);
    
    writeLastMoveTimestampIfDirty(statePacket, clientProxy);
    
    ReplicationManagerTransmissionData* rmtd = _replicationManagerTransmissionDatas.obtain();
    rmtd->reset(clientProxy->getReplicationManagerServer(), _entityManager, &_replicationManagerTransmissionDatas);
    
#if IS_DEBUG
    LOG("Pre-State Outgoing packet Bit Length: %d \n", statePacket.getBitLength());
#endif
    
    clientProxy->getReplicationManagerServer()->write(statePacket, rmtd);
    
    TransmissionData* currentTransmissionData = ifp->getTransmissionData('RPLM');
    if (currentTransmissionData)
    {
        _replicationManagerTransmissionDatas.free(static_cast<ReplicationManagerTransmissionData*>(currentTransmissionData));
    }
    
    ifp->setTransmissionData('RPLM', rmtd);
    
#if IS_DEBUG
    LOG("Post-State Outgoing packet Bit Length: %d \n", statePacket.getBitLength());
#endif
    
    sendPacket(statePacket, clientProxy->getMachineAddress());
}

void NetworkManagerServer::writeLastMoveTimestampIfDirty(OutputMemoryBitStream& outputStream, ClientProxy* clientProxy)
{
    bool isTimestampDirty = clientProxy->isLastMoveTimestampDirty();
    outputStream.write(isTimestampDirty);
    if (isTimestampDirty)
    {
        float lastProcessedMoveTimestamp = clientProxy->getUnprocessedMoveList().getLastProcessedMoveTimestamp();
        
        outputStream.write(lastProcessedMoveTimestamp);
        
        // which map are we on?
        outputStream.write(_map);
        
        clientProxy->setLastMoveTimestampDirty(false);
    }
}

void NetworkManagerServer::handleInputPacket(ClientProxy* clientProxy, InputMemoryBitStream& inputStream)
{
    uint8_t moveCount = 0;
    inputStream.read<uint8_t, 2>(moveCount);
    
	InputState* referenceInputState = NULL;
	bool isRefInputStateOrphaned = false;
    
    for (; moveCount > 0; --moveCount)
    {
        Move move = Move(_inputStateCreationFunc());
        
        bool isCopy;
        inputStream.read(isCopy);
        if (isCopy)
        {
            if (!referenceInputState)
            {
                LOG("Unexpected Network State!");
                
                return;
            }
            
            float timeStamp;
            inputStream.read(timeStamp);
            move.setTimestamp(timeStamp);
            
            move.copyInputState(referenceInputState);
        }
        else
        {
            move.read(inputStream);
        }

		if (isRefInputStateOrphaned && referenceInputState)
		{
            _inputStateReleaseFunc(referenceInputState);
            referenceInputState = NULL;
		}

		referenceInputState = move.getInputState();
        
        if (clientProxy->getUnprocessedMoveList().addMoveIfNew(move))
        {
			isRefInputStateOrphaned = false;
        }
		else
		{
			isRefInputStateOrphaned = true;
		}
    }
    
    if (isRefInputStateOrphaned && referenceInputState)
    {
        _inputStateReleaseFunc(referenceInputState);
        referenceInputState = NULL;
    }
}

void NetworkManagerServer::handleAddLocalPlayerPacket(ClientProxy* clientProxy, InputMemoryBitStream& inputStream)
{
    if (_playerIDToClientMap.size() < _serverHelper->maxNumPlayers())
    {
        // read the current number of local players for this client at the time when the request was made
        uint8_t requestedIndex;
        inputStream.read(requestedIndex);
        
        uint8_t playerID = clientProxy->getPlayerID(requestedIndex);
        if (playerID == NW_INPUT_UNASSIGNED)
        {
            std::string localPlayerName = StringUtil::format("%s(%d)", clientProxy->getName().c_str(), requestedIndex);
            
            uint8_t playerID = _nextPlayerID;
            
            clientProxy->onLocalPlayerAdded(playerID);
            
            _playerIDToClientMap[playerID] = clientProxy;
            
            // tell the server about this client
            _handleNewClientFunc(playerID, localPlayerName);
            
            updateNextPlayerID();
        }
        
        // and welcome the new local player...
        sendLocalPlayerAddedPacket(clientProxy, clientProxy->getNumPlayers() - 1);
    }
    else
    {
        OutputMemoryBitStream packet;
        packet.write(static_cast<uint8_t>(NWPT_LOCAL_PLAYER_DENIED));
        
        sendPacket(packet, clientProxy->getMachineAddress());
    }
}

void NetworkManagerServer::sendLocalPlayerAddedPacket(ClientProxy* clientProxy, uint8_t index)
{
    uint8_t playerID = clientProxy->getPlayerID(index);
    
    OutputMemoryBitStream packet;
    
    packet.write(static_cast<uint8_t>(NWPT_LOCAL_PLAYER_ADDED));
    packet.write<uint8_t, 3>(playerID);
    
    std::string localPlayerName = StringUtil::format("%s(%d)", clientProxy->getName().c_str(), index);
    
    LOG("Server welcoming new client local player '%s' as player %d", localPlayerName.c_str(), playerID);
    
    sendPacket(packet, clientProxy->getMachineAddress());
}

void NetworkManagerServer::handleDropLocalPlayerPacket(ClientProxy* clientProxy, InputMemoryBitStream& inputStream)
{
    // read the index to drop
    uint8_t index;
    inputStream.read(index);
    
    if (index < 1)
    {
        // If the primary player on this client wants to drop, a disconnect request should be fired off instead of a drop
        return;
    }
    
    uint8_t playerID = clientProxy->getPlayerID(index);
    if (playerID != NW_INPUT_UNASSIGNED)
    {
        _playerIDToClientMap.erase(playerID);
        
        _handleLostClientFunc(clientProxy, index);
        
        clientProxy->onLocalPlayerRemoved(playerID);
        
        updateNextPlayerID();
    }
}

void NetworkManagerServer::handleClientDisconnected(ClientProxy* clientProxy)
{
    for (uint8_t i = 0; i < clientProxy->getNumPlayers(); ++i)
    {
        _playerIDToClientMap.erase(clientProxy->getPlayerID(i));
    }
    
    _addressHashToClientMap.erase(clientProxy->getMachineAddress()->getHash());
    
    _handleLostClientFunc(clientProxy, 0);
    
    _serverHelper->onClientDisconnected(clientProxy);
    
    delete clientProxy;
    
    updateNextPlayerID();
}

void NetworkManagerServer::updateNextPlayerID()
{
    LOG("updateNextPlayerID");
    
    // Find the next available Player ID
    _nextPlayerID = 1;
    for (uint8_t i = 0; i < _serverHelper->maxNumPlayers(); ++i)
    {
        for (auto const& entry : _playerIDToClientMap)
        {
            if (entry.first == _nextPlayerID)
            {
                ++_nextPlayerID;
            }
        }
    }
    
    LOG("_nextPlayerID: %d", _nextPlayerID);
}

NetworkManagerServer::NetworkManagerServer(ServerHelper* serverHelper, HandleNewClientFunc handleNewClientFunc, HandleLostClientFunc handleLostClientFunc, InputStateCreationFunc inputStateCreationFunc, InputStateReleaseFunc inputStateReleaseFunc) :
_serverHelper(serverHelper),
_handleNewClientFunc(handleNewClientFunc),
_handleLostClientFunc(handleLostClientFunc),
_inputStateCreationFunc(inputStateCreationFunc),
_inputStateReleaseFunc(inputStateReleaseFunc),
_entityManager(new EntityManager(NULL, NULL)),
_timeTracker(INSTANCE_MGR.get<TimeTracker>(INSK_TIMING_SERVER)),
_nextPlayerID(1),
_map(0)
{
    // Empty
}

NetworkManagerServer::~NetworkManagerServer()
{
    delete _serverHelper;
    delete _entityManager;
    
    GowUtil::cleanUpMapOfPointers(_addressHashToClientMap);
    
    _playerIDToClientMap.clear();
}
