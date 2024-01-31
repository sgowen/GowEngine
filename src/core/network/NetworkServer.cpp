//
//  NetworkServer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

NetworkServer* NetworkServer::s_instance = nullptr;

void NetworkServer::create(ServerHelper* serverHelper, EntityIDManager& eidm, TimeTracker& tt, OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf, HandleNewClientFunc hncf, HandleLostClientFunc hlcf)
{
    assert(s_instance == nullptr);
    
    s_instance = new NetworkServer(serverHelper, eidm, tt, oerf, oedf, hncf, hlcf);
    
    assert(NW_SRVR != nullptr);
}

NetworkServer* NetworkServer::getInstance()
{
    return s_instance;
}

void NetworkServer::destroy()
{
    assert(s_instance != nullptr);
    
    delete s_instance;
    s_instance = nullptr;
}

void NetworkServer::sProcessPacket(InputMemoryBitStream& inputStream, MachineAddress* inFromAddress)
{
    NW_SRVR->processPacket(inputStream, inFromAddress);
}

ClientProxy* NetworkServer::sGetClientProxy(uint8_t inPlayerIndex)
{
    return NW_SRVR->getClientProxy(inPlayerIndex + 1);
}

void NetworkServer::sHandleClientDisconnected(ClientProxy* inClientProxy)
{
    ClientProxy& cp = *inClientProxy;
    
    NW_SRVR->handleClientDisconnected(cp);
}

int NetworkServer::processIncomingPackets()
{
    _serverHelper->processIncomingPackets();
    
    std::vector<ClientProxy*> clientsToDC;
    
    uint32_t timeout = ENGINE_CFG.framesPerSecond() / 2;
    uint32_t minAllowedLastPacketFromClientTime = _timeTracker._time < timeout ? 0 : _timeTracker._time - timeout;
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
    
    return getNumMovesReadyToBeProcessed();
}

void NetworkServer::sendOutgoingPackets()
{
    for (auto& pair: _addressHashToClientMap)
    {
        ClientProxy& cp = pair.second;
        cp.getDeliveryNotificationManager().processTimedOutPackets();
        
        sendStatePacketToClient(cp);
    }
}

void NetworkServer::registerEntity(Entity* e)
{
    _entityRegistry.registerEntity(e);
}

void NetworkServer::registerNewEntity(uint32_t key, uint32_t x, uint32_t y)
{
    EntityInstanceDef eid(_entityIDManager.getNextNetworkEntityID(), key, x, y, true);
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
        pair.second.replicationManagerServer().setStateDirty(networkID, dirtyState);
    }
}

ClientProxy* NetworkServer::getClientProxy(uint8_t playerID) const
{
    auto it = _playerIDToClientMap.find(playerID);
    if (it != _playerIDToClientMap.end())
    {
        return it->second;
    }
    
    return nullptr;
}

uint8_t NetworkServer::getNumClientsConnected()
{
    return static_cast<uint8_t>(_addressHashToClientMap.size());
}

uint8_t NetworkServer::getNumPlayersConnected()
{
    return static_cast<uint8_t>(_playerIDToClientMap.size());
}

MachineAddress* NetworkServer::getServerAddress()
{
    return _serverHelper->getServerAddress();
}

bool NetworkServer::isConnected()
{
    return _serverHelper->isConnected();
}

bool NetworkServer::connect()
{
    int error = _serverHelper->connect();
    
    if (error != NO_ERROR &&
        ENGINE_CFG.networkLoggingEnabled())
    {
        LOG("Server connect failed. Error code %d", error);
    }
    
    return error == NO_ERROR;
}

void NetworkServer::onEntityRegistered(Entity* e)
{
    for (auto& pair: _addressHashToClientMap)
    {
        pair.second.replicationManagerServer().replicateCreate(e->getID());
    }
    
    _onEntityRegisteredFunc(e);
}

void NetworkServer::onEntityDeregistered(Entity* e)
{
    for (auto& pair: _addressHashToClientMap)
    {
        pair.second.replicationManagerServer().replicateDestroy(e->getID());
    }
    
    _onEntityDeregisteredFunc(e);
}

void NetworkServer::processPacket(InputMemoryBitStream& imbs, MachineAddress* fromAddress)
{
    if (ENGINE_CFG.networkLoggingEnabled())
    {
        LOG("Server processPacket bit length: %d", imbs.getRemainingBitCount());
    }
    
    auto it = _addressHashToClientMap.find(fromAddress->getHash());
    if (it == _addressHashToClientMap.end())
    {
        if (_playerIDToClientMap.size() < MAX_NUM_PLAYERS)
        {
            if (ENGINE_CFG.networkLoggingEnabled())
            {
                LOG("Server is processing new client from %s", fromAddress->toString().c_str());
            }
            
            handlePacketFromNewClient(imbs, fromAddress);
        }
        else
        {
            if (ENGINE_CFG.networkLoggingEnabled())
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

void NetworkServer::removeProcessedMoves()
{
    for (auto& pair : _playerIDToClientMap)
    {
        ClientProxy* cp = pair.second;
        assert(cp != nullptr);
        
        MoveList& ml = cp->getUnprocessedMoveList();
        ml.removeProcessedMoves(_poolInputState);
    }
}

void NetworkServer::setNumMovesProcessed(uint32_t numMovesProcessed)
{
    _numMovesProcessed = numMovesProcessed;
}

const std::map<int, ClientProxy*>& NetworkServer::playerIDToClientMap()
{
    return _playerIDToClientMap;
}

ServerHelper* NetworkServer::getServerHelper()
{
    return _serverHelper;
}

int NetworkServer::getNumMovesReadyToBeProcessed()
{
    int lowestMoveCount = 0;

    ClientProxy* cp = getClientProxy(1);
    if (cp != nullptr)
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

    uint32_t expectedMoveStartIndex = _numMovesProcessed;
    for (int i = 0; i < lowestMoveCount; ++i)
    {
        uint32_t expectedMoveIndex = expectedMoveStartIndex + i;
        
        for (auto& pair : _playerIDToClientMap)
        {
            ClientProxy* cp = pair.second;
            assert(cp != nullptr);

            MoveList& ml = cp->getUnprocessedMoveList();
            Move* m = ml.getMoveWithMoveIndex(expectedMoveIndex);
            if (m == nullptr)
            {
                if (ENGINE_CFG.networkLoggingEnabled())
                {
                    LOG("Player %s is missing move at index: %d", cp->getUsername().c_str(), expectedMoveIndex);
                }
                return i;
            }
        }
    }

    return lowestMoveCount;
}

void NetworkServer::sendPacket(const OutputMemoryBitStream& ombs, MachineAddress* fromAddress)
{
    if (ENGINE_CFG.networkLoggingEnabled())
    {
        LOG("Server    sendPacket bit length: %d", ombs.getBitLength());
    }
    
    _serverHelper->sendPacket(ombs, fromAddress);
}

void NetworkServer::handlePacketFromNewClient(InputMemoryBitStream& imbs, MachineAddress* fromAddress)
{
    uint8_t packetType;
    imbs.readBits(packetType, 4);
    
    if (packetType == NWPT_HELLO)
    {
        std::string name;
        imbs.readSmall(name);
        
        _addressHashToClientMap.emplace(std::piecewise_construct,
                  std::forward_as_tuple(fromAddress->getHash()),
                  std::forward_as_tuple(_entityRegistry, _timeTracker, fromAddress, name, _nextPlayerID));
        ClientProxy& cp = _addressHashToClientMap.at(fromAddress->getHash());
        std::string playerName = cp.getUsername();
        uint8_t playerID = cp.getPlayerID();
        _playerIDToClientMap[playerID] = &cp;
        
        _handleNewClientFunc(playerName, playerID);
        
        sendWelcomePacket(cp);
        
        for (auto& pair: _entityRegistry.getMap())
        {
            cp.replicationManagerServer().replicateCreate(pair.first);
        }
        
        resetNextPlayerID();
    }
    else
    {
        if (ENGINE_CFG.networkLoggingEnabled())
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
            if (ENGINE_CFG.networkLoggingEnabled())
            {
                LOG("Unknown packet type received from %s", cp.getMachineAddress()->toString().c_str());
            }
            break;
    }
}

void NetworkServer::sendWelcomePacket(ClientProxy& cp)
{
    OutputMemoryBitStream ombs(1);
    ombs.writeBits(static_cast<uint8_t>(NWPT_WELCOME), 4);
    ombs.writeBits(cp.getPlayerID(), 3);
    sendPacket(ombs, cp.getMachineAddress());
    
    if (ENGINE_CFG.networkLoggingEnabled())
    {
        LOG("Server welcoming new client '%s' as player %d", cp.getUsername().c_str(), cp.getPlayerID());
    }
}

void NetworkServer::sendStatePacketToClient(ClientProxy& cp)
{
    OutputMemoryBitStream ombs(NW_MAX_PACKET_SIZE);
    ombs.writeBits(static_cast<uint8_t>(NWPT_STATE), 4);
    
    InFlightPacket* ifp = cp.getDeliveryNotificationManager().writeState(ombs);
    
    ombs.write(_numMovesProcessed);
    
    bool isTimestampDirty = cp.isLastMoveTimestampDirty();
    ombs.write(isTimestampDirty);
    if (isTimestampDirty)
    {
        uint32_t lastProcessedMoveTimestamp = cp.getUnprocessedMoveList().getLastProcessedMoveTimestamp();
        ombs.write(lastProcessedMoveTimestamp);
        cp.setLastMoveTimestampDirty(false);
    }
    
    ReplicationTransmissionData* rtd = _poolRMTD.obtain();
    rtd->reset(&cp.replicationManagerServer(), &_entityRegistry, &_poolRMTD);
    
    cp.replicationManagerServer().write(ombs, rtd);
    
    TransmissionData* td = ifp->getTransmissionData('RPLM');
    if (td != nullptr)
    {
        td->free();
    }
    
    ifp->setTransmissionData('RPLM', rtd);
    
    sendPacket(ombs, cp.getMachineAddress());
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
    
    static uint8_t maxNumFramesOfRollback = ENGINE_CFG.maxNumFramesOfRollback();
    static uint8_t numFramesOfInputDelay = ENGINE_CFG.numFramesOfInputDelay();
    static uint8_t maxNumMoves = numFramesOfInputDelay + maxNumFramesOfRollback + 1;
    
    uint8_t moveCount = 0;
    imbs.readBits(moveCount, NBITS(maxNumMoves));
    
    for (; moveCount > 0; --moveCount)
    {
        InputState* is = _poolInputState.obtain();
        is->reset();
        Move move = Move(is);
        move.read(imbs);

        if (!cp.getUnprocessedMoveList().addMoveIfNew(move))
        {
            _poolInputState.free(is);
        }
    }
}

void NetworkServer::handleAddLocalPlayerPacket(ClientProxy& cp, InputMemoryBitStream& imbs)
{
    if (_playerIDToClientMap.size() < MAX_NUM_PLAYERS)
    {
        uint8_t requestedIndex;
        imbs.read(requestedIndex);
        
        uint8_t playerID = cp.getPlayerID(requestedIndex);
        if (playerID == 0)
        {
            std::string localUsername = STRING_FORMAT("%s(%d)", cp.getUsername().c_str(), requestedIndex);
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
        sendPacket(ombs, cp.getMachineAddress());
    }
}

void NetworkServer::sendLocalPlayerAddedPacket(ClientProxy& cp)
{
    uint8_t index = cp.getNumPlayers() - 1;
    uint8_t playerID = cp.getPlayerID(index);
    
    OutputMemoryBitStream ombs(1);
    ombs.writeBits(static_cast<uint8_t>(NWPT_LOCAL_PLAYER_ADDED), 4);
    ombs.writeBits(playerID, 3);
    sendPacket(ombs, cp.getMachineAddress());
    
    std::string localPlayerName = STRING_FORMAT("%s(%d)", cp.getUsername().c_str(), index);
    if (ENGINE_CFG.networkLoggingEnabled())
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
    if (ENGINE_CFG.networkLoggingEnabled())
    {
        LOG("Client is leaving the server");
    }
    
    for (uint8_t i = 0; i < cp.getNumPlayers(); ++i)
    {
        _playerIDToClientMap.erase(cp.getPlayerID(i));
    }
    
    MoveList& ml = cp.getUnprocessedMoveList();
    ml.removeAllMoves(_poolInputState);
    
    _handleLostClientFunc(cp, 0);
    
    _addressHashToClientMap.erase(cp.getMachineAddress()->getHash());
    
    resetNextPlayerID();
    
    if (getNumClientsConnected() == 0)
    {
        deregisterAllEntities();
        
        _entityIDManager.resetNextNetworkEntityID();
        _entityIDManager.resetNextPlayerEntityID();
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

void cb_nw_srvr_processPacket(InputMemoryBitStream& imbs, MachineAddress* fromAddress)
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

NetworkServer::NetworkServer(ServerHelper* serverHelper, EntityIDManager& eidm, TimeTracker& tt, OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf, HandleNewClientFunc hncf, HandleLostClientFunc hlcf) :
_serverHelper(serverHelper),
_entityIDManager(eidm),
_timeTracker(tt),
_onEntityRegisteredFunc(oerf),
_onEntityDeregisteredFunc(oedf),
_handleNewClientFunc(hncf),
_handleLostClientFunc(hlcf),
_entityRegistry(cb_nw_srvr_onEntityRegistered, cb_nw_srvr_onEntityDeregistered),
_nextPlayerID(1),
_numMovesProcessed(0)
{
    // Empty 
}

NetworkServer::~NetworkServer()
{
    for (auto& pair : _addressHashToClientMap)
    {
        ClientProxy& cp = pair.second;
        
        MoveList& ml = cp.getUnprocessedMoveList();
        ml.removeAllMoves(_poolInputState);
        
        OutputMemoryBitStream ombs(1);
        ombs.writeBits(static_cast<uint8_t>(NWPT_SRVR_EXIT), 4);
        sendPacket(ombs, cp.getMachineAddress());
    }

    _addressHashToClientMap.clear();
    _playerIDToClientMap.clear();
    
    deregisterAllEntities();
}
