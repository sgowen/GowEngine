//
//  NetworkServer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

NetworkServer* NetworkServer::s_instance = NULL;

void NetworkServer::create(uint16_t port, uint8_t maxNumPlayers, OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf, HandleNewClientFunc hncf, HandleLostClientFunc hlcf)
{
    assert(s_instance == NULL);
    
    INST_REG.get<TimeTracker>(INSK_TIME_SRVR)->reset();
    INST_REG.get<EntityIDManager>(INSK_EID_SRVR)->resetNextNetworkEntityID();
    INST_REG.get<EntityIDManager>(INSK_EID_SRVR)->resetNextPlayerEntityID();
    
    s_instance = new NetworkServer(port, maxNumPlayers, oerf, oedf, hncf, hlcf);
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

void NetworkServer::registerNewEntity(uint32_t key, uint32_t x, uint32_t y)
{
    EntityInstanceDef eid(INST_REG.get<EntityIDManager>(INSK_EID_SRVR)->getNextNetworkEntityID(), key, x, y, true);
    Entity* e = ENTITY_MGR.createEntity(eid);
    registerEntity(e);
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
    if (IS_NETWORK_LOGGING_ENABLED())
    {
        LOG("Server Initializing PacketHandler at port %hu", _port);
    }
    
    int error = _packetHandler.connect();
    if (error != NO_ERROR &&
        IS_NETWORK_LOGGING_ENABLED())
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

void NetworkServer::processPacket(InputMemoryBitStream& imbs, SocketAddress* fromAddress)
{
    if (IS_NETWORK_LOGGING_ENABLED())
    {
        LOG("Server processPacket bit length: %d", imbs.getRemainingBitCount());
    }
    
    auto it = _addressHashToClientMap.find(fromAddress->getHash());
    if (it == _addressHashToClientMap.end())
    {
        if (_playerIDToClientMap.size() < _maxNumPlayers)
        {
            if (IS_NETWORK_LOGGING_ENABLED())
            {
                LOG("Server is processing new client from %s", fromAddress->toString().c_str());
            }
            
            handlePacketFromNewClient(imbs, fromAddress);
        }
        else
        {
            if (IS_NETWORK_LOGGING_ENABLED())
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

void NetworkServer::removeProcessedMovesForPlayer(uint8_t playerID)
{
    ClientProxy* cp = NW_SRVR->getClientProxy(playerID);
    assert(cp != NULL);

    MoveList& ml = cp->getUnprocessedMoveList();
    ml.removeProcessedMoves(cb_inputStateRelease);
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
    if (IS_NETWORK_LOGGING_ENABLED())
    {
        LOG("Server    sendPacket bit length: %d", ombs.getBitLength());
    }
    
    _packetHandler.sendPacket(ombs, fromAddress);
}

void NetworkServer::handlePacketFromNewClient(InputMemoryBitStream& imbs, SocketAddress* fromAddress)
{
    uint8_t packetType;
    imbs.readBits(packetType, 4);
    
    if (packetType == NWPT_HELLO)
    {
        std::string name;
        imbs.readSmall(name);
        
        _addressHashToClientMap.emplace(std::piecewise_construct,
                  std::forward_as_tuple(fromAddress->getHash()),
                  std::forward_as_tuple(_entityRegistry, fromAddress, name, _nextPlayerID));
        ClientProxy& cp = _addressHashToClientMap.at(fromAddress->getHash());
        std::string username = cp.getUsername();
        uint8_t playerID = cp.getPlayerID();
        _playerIDToClientMap[playerID] = &cp;
        
        _handleNewClientFunc(username, playerID);
        
        sendWelcomePacket(cp);
        
        for (auto& pair: _entityRegistry.getMap())
        {
            cp.getReplicationManagerServer().replicateCreate(pair.first, ALL_DIRTY_STATE);
        }
        
        resetNextPlayerID();
    }
    else
    {
        if (IS_NETWORK_LOGGING_ENABLED())
        {
            LOG("Unknown packet type received from %s", fromAddress->toString().c_str());
        }
    }
}

void NetworkServer::processPacket(ClientProxy& cp, InputMemoryBitStream& imbs)
{
    cp.updateLastPacketTime();
    
    uint8_t packetType;
    imbs.readBits(packetType, 4);
    
    switch (packetType)
    {
        case NWPT_HELLO:
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
            if (IS_NETWORK_LOGGING_ENABLED())
            {
                LOG("Unknown packet type received from %s", cp.getSocketAddress()->toString().c_str());
            }
            break;
    }
}

void NetworkServer::sendWelcomePacket(ClientProxy& cp)
{
    OutputMemoryBitStream ombs(1);
    ombs.writeBits(static_cast<uint8_t>(NWPT_WELCOME), 4);
    ombs.writeBits(cp.getPlayerID(), 3);
    sendPacket(ombs, cp.getSocketAddress());
    
    if (IS_NETWORK_LOGGING_ENABLED())
    {
        LOG("Server welcoming new client '%s' as player %d", cp.getUsername().c_str(), cp.getPlayerID());
    }
}

void NetworkServer::sendStatePacketToClient(ClientProxy& cp)
{
    OutputMemoryBitStream ombs(NW_MAX_PACKET_SIZE);
    ombs.writeBits(static_cast<uint8_t>(NWPT_STATE), 4);
    
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
    imbs.readBits(moveCount, 4);
    
	InputState* referenceInputState = NULL;
	bool isRefInputStateOrphaned = false;
    
    for (; moveCount > 0; --moveCount)
    {
        InputState* is = _poolInputState.obtain();
        is->reset();
        Move move = Move(is);
        
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
            cb_inputStateRelease(referenceInputState);
		}

		referenceInputState = move.inputState();
        isRefInputStateOrphaned = !cp.getUnprocessedMoveList().addMoveIfNew(move);
    }
    
    if (isRefInputStateOrphaned)
    {
        cb_inputStateRelease(referenceInputState);
    }
}

void NetworkServer::handleAddLocalPlayerPacket(ClientProxy& cp, InputMemoryBitStream& imbs)
{
    if (_playerIDToClientMap.size() < _maxNumPlayers)
    {
        uint8_t requestedIndex;
        imbs.read(requestedIndex);
        
        uint8_t playerID = cp.getPlayerID(requestedIndex);
        if (playerID == 0)
        {
            std::string localUsername = StringUtil::format("%s(%d)", cp.getUsername().c_str(), requestedIndex);
            uint8_t playerID = _nextPlayerID;
            
            cp.onLocalPlayerAdded(playerID);
            
            _playerIDToClientMap[playerID] = &cp;
            
            _handleNewClientFunc(localUsername, playerID);
            
            resetNextPlayerID();
        }
        
        sendLocalPlayerAddedPacket(cp);
    }
    else
    {
        OutputMemoryBitStream ombs(1);
        ombs.writeBits(static_cast<uint8_t>(NWPT_LOCAL_PLAYER_DENIED), 4);
        sendPacket(ombs, cp.getSocketAddress());
    }
}

void NetworkServer::sendLocalPlayerAddedPacket(ClientProxy& cp)
{
    uint8_t index = cp.getNumPlayers() - 1;
    uint8_t playerID = cp.getPlayerID(index);
    
    OutputMemoryBitStream ombs(1);
    ombs.writeBits(static_cast<uint8_t>(NWPT_LOCAL_PLAYER_ADDED), 4);
    ombs.writeBits(playerID, 3);
    sendPacket(ombs, cp.getSocketAddress());
    
    std::string localPlayerName = StringUtil::format("%s(%d)", cp.getUsername().c_str(), index);
    if (IS_NETWORK_LOGGING_ENABLED())
    {
        LOG("Server welcoming new client local player '%s' as player %d", localPlayerName.c_str(), playerID);
    }
}

void NetworkServer::handleDropLocalPlayerPacket(ClientProxy& cp, InputMemoryBitStream& imbs)
{
    uint8_t localPlayerIndex;
    imbs.read(localPlayerIndex);
    
    assert(localPlayerIndex >= 1);
    
    uint8_t playerID = cp.getPlayerID(localPlayerIndex);
    if (playerID == 0)
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
    if (IS_NETWORK_LOGGING_ENABLED())
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
        
        INST_REG.get<TimeTracker>(INSK_TIME_SRVR)->reset();
        INST_REG.get<EntityIDManager>(INSK_EID_SRVR)->resetNextNetworkEntityID();
        INST_REG.get<EntityIDManager>(INSK_EID_SRVR)->resetNextPlayerEntityID();
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

NetworkServer::NetworkServer(uint16_t port, uint8_t maxNumPlayers, OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf, HandleNewClientFunc hncf, HandleLostClientFunc hlcf) :
_packetHandler(INST_REG.get<TimeTracker>(INSK_TIME_SRVR), port, cb_nw_srvr_processPacket),
_onEntityRegisteredFunc(oerf),
_onEntityDeregisteredFunc(oedf),
_handleNewClientFunc(hncf),
_handleLostClientFunc(hlcf),
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
        
        OutputMemoryBitStream ombs(1);
        ombs.writeBits(static_cast<uint8_t>(NWPT_SRVR_EXIT), 4);
        sendPacket(ombs, cp.getSocketAddress());
    }
    
    _addressHashToClientMap.clear();
    _playerIDToClientMap.clear();
    
    deregisterAllEntities();
}
