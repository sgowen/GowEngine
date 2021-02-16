//
//  NetworkManagerClient.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "NetworkManagerClient.hpp"

#include "InputMemoryBitStream.hpp"
#include "OutputMemoryBitStream.hpp"
#include "SocketAddress.hpp"
#include "Entity.hpp"
#include "MoveList.hpp"
#include "SocketAddress.hpp"
#include "TimeTracker.hpp"
#include "StringUtil.hpp"
#include "SocketAddressFactory.hpp"
#include "Macros.hpp"
#include "InstanceRegistry.hpp"
#include "SocketUtil.hpp"

#include <assert.h>

NetworkManagerClient* NetworkManagerClient::s_instance = NULL;

void NetworkManagerClient::create(std::string serverIPAddress, std::string username, uint16_t port, OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf, RemoveProcessedMovesFunc rpmf, GetMoveListFunc gmlf, OnPlayerWelcomedFunc opwf)
{
    assert(s_instance == NULL);
    
    INST_REG.get<TimeTracker>(INSK_TIME_CLNT)->reset();
    
    s_instance = new NetworkManagerClient(serverIPAddress, username, port, oerf, oedf, rpmf, gmlf, opwf);
}

NetworkManagerClient * NetworkManagerClient::getInstance()
{
    return s_instance;
}

void NetworkManagerClient::destroy()
{
    assert(s_instance != NULL);
    
    delete s_instance;
    s_instance = NULL;
}

void NetworkManagerClient::processIncomingPackets()
{
    _hasReceivedNewState = false;
    
    _packetHandler.processIncomingPackets();
    
    TimeTracker* tt = INST_REG.get<TimeTracker>(INSK_TIME_CLNT);
    uint32_t time = tt->_time;
    uint32_t dcTime = _lastServerCommunicationTimestamp + NW_SRVR_TIMEOUT;
    if (time > dcTime)
    {
        _state = NWCS_DISCONNECTED;
    }
}

void NetworkManagerClient::sendOutgoingPackets()
{
    switch (_state)
    {
        case NWCS_SAYING_HELLO:
            updateSayingHello();
            break;
        case NWCS_WELCOMED:
            updateSendingInputPacket();
            updateAddLocalPlayerRequest();
            updateDropLocalPlayerRequest();
            break;
        case NWCS_DISCONNECTED:
        default:
            break;
    }
}

void NetworkManagerClient::requestToAddLocalPlayer()
{
    _isRequestingToAddLocalPlayer = true;
    _isRequestingToDropLocalPlayer = 0;
}

void NetworkManagerClient::requestToDropLocalPlayer(uint8_t index)
{
    assert(index >= 1);
    
    _isRequestingToAddLocalPlayer = false;
    _isRequestingToDropLocalPlayer = index;
    _indexToPlayerIDMap.erase(index);
    updateNextIndex();
}

const WeightedTimedMovingAverage& NetworkManagerClient::getBytesReceivedPerSecond() const
{
    return _packetHandler.getBytesReceivedPerSecond();
}

const WeightedTimedMovingAverage& NetworkManagerClient::getBytesSentPerSecond() const
{
    return _packetHandler.getBytesSentPerSecond();
}

const WeightedTimedMovingAverage& NetworkManagerClient::getAvgRoundTripTime() const
{
    return _avgRoundTripTime;
}

float NetworkManagerClient::getRoundTripTime() const
{
    return _avgRoundTripTime.getValue();
}

bool NetworkManagerClient::isPlayerIDLocal(uint8_t playerID) const
{
    for (auto const& entry : _indexToPlayerIDMap)
    {
        if (entry.second == playerID)
        {
            return true;
        }
    }
    
    return false;
}

bool NetworkManagerClient::hasReceivedNewState()
{
    return _hasReceivedNewState;
}

std::map<uint8_t, uint8_t>& NetworkManagerClient::getPlayerIDs()
{
    return _indexToPlayerIDMap;
}

std::string& NetworkManagerClient::getPlayerName()
{
    return _username;
}

NetworkClientState NetworkManagerClient::state() const
{
    return _state;
}

bool NetworkManagerClient::connect()
{
    if (SOCKET_UTIL.isLoggingEnabled())
    {
        LOG("Client Initializing PacketHandler at port %hu", _port);
    }
    
    int error = _packetHandler.connect();
    if (error != NO_ERROR &&
        SOCKET_UTIL.isLoggingEnabled())
    {
        LOG("Client connect failed. Error code %d", error);
    }
    
    return error == NO_ERROR;
}

EntityRegistry& NetworkManagerClient::getEntityRegistry()
{
    return _entityRegistry;
}

void NetworkManagerClient::processPacket(InputMemoryBitStream& imbs, SocketAddress* fromAddress)
{
    if (SOCKET_UTIL.isLoggingEnabled())
    {
        LOG("Client processPacket bit length: %d", imbs.getRemainingBitCount());
    }
    
    TimeTracker* tt = INST_REG.get<TimeTracker>(INSK_TIME_CLNT);
    _lastServerCommunicationTimestamp = tt->_time;
    
    uint8_t packetType;
    imbs.read<uint8_t, 4>(packetType);
    
    switch (packetType)
    {
        case NWPT_WELCOME:
            handleWelcomePacket(imbs);
            break;
        case NWPT_LOCAL_PLAYER_ADDED:
            handleLocalPlayerAddedPacket(imbs);
            break;
        case NWPT_LOCAL_PLAYER_DENIED:
            handleLocalPlayerDeniedPacket();
            break;
        case NWPT_STATE:
            handleStatePacket(imbs);
            break;
        case NWPT_SRVR_EXIT:
            if (SOCKET_UTIL.isLoggingEnabled())
            {
                LOG("Server has shut down");
            }
            _state = NWCS_DISCONNECTED;
            break;
        default:
            if (SOCKET_UTIL.isLoggingEnabled())
            {
                LOG("Unknown packet type received from %s", fromAddress->toString().c_str());
            }
            break;
    }
}

void NetworkManagerClient::onMoveProcessed()
{
    ++_numMovesProcessed;
}

uint32_t NetworkManagerClient::getNumMovesProcessed()
{
    return _numMovesProcessed;
}

void NetworkManagerClient::sendPacket(const OutputMemoryBitStream& ombs)
{
    if (SOCKET_UTIL.isLoggingEnabled())
    {
        LOG("Client    sendPacket bit length: %d", ombs.getBitLength());
    }
    
    _packetHandler.sendPacket(ombs, _serverAddress);
}

void NetworkManagerClient::updateSayingHello()
{
    TimeTracker* tt = INST_REG.get<TimeTracker>(INSK_TIME_CLNT);
    float time = tt->_time;
    
    if (time > _timeOfLastHello + NW_CLNT_TIME_BETWEEN_HELLOS)
    {
        OutputMemoryBitStream ombs;
        ombs.write<uint8_t, 4>(static_cast<uint8_t>(NWPT_HELLO));
        ombs.writeSmall(getPlayerName());
        sendPacket(ombs);
        
        _timeOfLastHello = time;
    }
}

void NetworkManagerClient::handleWelcomePacket(InputMemoryBitStream& imbs)
{
    if (_state != NWCS_SAYING_HELLO)
    {
        return;
    }
    
    // if we got a player ID, we've been welcomed!
    uint8_t playerID;
    imbs.read<uint8_t, 3>(playerID);
    
    _state = NWCS_WELCOMED;
    
    _indexToPlayerIDMap.clear();
    _indexToPlayerIDMap[_nextIndex] = playerID;
    
    if (SOCKET_UTIL.isLoggingEnabled())
    {
        LOG("Client '%s' was welcomed as player %d", getPlayerName().c_str(), playerID);
    }
    
    _onPlayerWelcomedFunc(playerID);
    
    updateNextIndex();
}

void NetworkManagerClient::handleLocalPlayerAddedPacket(InputMemoryBitStream& imbs)
{
    if (_state != NWCS_WELCOMED || !_isRequestingToAddLocalPlayer)
    {
        return;
    }
    
    // if we got a player ID, our local player has been added!
    uint8_t playerID;
    imbs.read(playerID);
    
    _indexToPlayerIDMap[_nextIndex] = playerID;
    
    if (SOCKET_UTIL.isLoggingEnabled())
    {
        LOG("'%s(%d)' was welcomed on client as player %d", getPlayerName().c_str(), _nextIndex, playerID);
    }
    
    _isRequestingToAddLocalPlayer = false;
    
    _onPlayerWelcomedFunc(playerID);
    
    updateNextIndex();
}

void NetworkManagerClient::handleLocalPlayerDeniedPacket()
{
    if (SOCKET_UTIL.isLoggingEnabled())
    {
        LOG("'%s(%d)' was denied on client...", getPlayerName().c_str(), _nextIndex);
    }
    
    _isRequestingToAddLocalPlayer = false;
}

void NetworkManagerClient::handleStatePacket(InputMemoryBitStream& imbs)
{
    if (_state != NWCS_WELCOMED)
    {
        return;
    }
    
    imbs.read(_numMovesProcessed);
    
    if (!_deliveryNotificationManager.readAndProcessState(imbs))
    {
        return;
    }
    
    bool isTimestampDirty;
    imbs.read(isTimestampDirty);
    if (isTimestampDirty)
    {
        imbs.read(_lastMoveProcessedByServerTimestamp);
        
        TimeTracker* tt = INST_REG.get<TimeTracker>(INSK_TIME_CLNT);
        float rtt = tt->realTime() - tt->realTime(_lastMoveProcessedByServerTimestamp);
        _avgRoundTripTime.update(rtt);
        
        _removeProcessedMovesFunc(_lastMoveProcessedByServerTimestamp);
    }
    
    _replicationManagerClient.read(imbs, _entityRegistry);
    _hasReceivedNewState = true;
}

void NetworkManagerClient::updateSendingInputPacket()
{
    MoveList& moveList = _getMoveListFunc();
    
    OutputMemoryBitStream ombs;
    ombs.write<uint8_t, 4>(static_cast<uint8_t>(NWPT_INPUT));
    
    _deliveryNotificationManager.writeState(ombs);
    
    ombs.write(moveList.hasMoves());
    if (moveList.hasMoves())
    {
        int moveCount = moveList.getNumMovesAfterTimestamp(_lastMoveProcessedByServerTimestamp);
        assert(moveCount <= NW_CLNT_MAX_NUM_MOVES);
        ombs.write<uint8_t, 4>(moveCount);
        
        std::deque<Move>::const_iterator moveItr = moveList.begin();
        
        const Move* moveToCopy = NULL;
        for (int i = 0; i < moveCount; ++i, ++moveItr)
        {
            if (moveToCopy != NULL && moveItr->isEqual(moveToCopy))
            {
                ombs.write(true);
                ombs.write(moveItr->getTimestamp());
                ombs.write(moveItr->getIndex());
            }
            else
            {
                ombs.write(false);
                moveItr->write(ombs);
                
                moveToCopy = &(*moveItr);
            }
        }
    }
    else
    {
        if (SOCKET_UTIL.isLoggingEnabled())
        {
            LOG("Client has no moves this frame");
        }
    }
    
    sendPacket(ombs);
}

void NetworkManagerClient::updateAddLocalPlayerRequest()
{
    if (!_isRequestingToAddLocalPlayer)
    {
        return;
    }
    
    TimeTracker* tt = INST_REG.get<TimeTracker>(INSK_TIME_CLNT);
    float time = tt->_time;
    
    if (time > _timeOfLastHello + NW_CLNT_TIME_BETWEEN_HELLOS)
    {
        OutputMemoryBitStream ombs;
        ombs.write<uint8_t, 4>(static_cast<uint8_t>(NWPT_ADD_LOCAL_PLAYER));
        ombs.write(_nextIndex);
        sendPacket(ombs);
        
        _timeOfLastHello = time;
    }
}

void NetworkManagerClient::updateDropLocalPlayerRequest()
{
    if (_isRequestingToDropLocalPlayer == 0)
    {
        return;
    }
    
    _isRequestingToAddLocalPlayer = false;
    
    OutputMemoryBitStream ombs;
    ombs.write<uint8_t, 4>(static_cast<uint8_t>(NWPT_DROP_LOCAL_PLAYER));
    ombs.write(_isRequestingToDropLocalPlayer);
    sendPacket(ombs);
    
    _isRequestingToDropLocalPlayer = 0;
}

void NetworkManagerClient::updateNextIndex()
{
    // Find the next available Player ID
    _nextIndex = 0;
    for (auto const& entry : _indexToPlayerIDMap)
    {
        if (entry.first == _nextIndex)
        {
            ++_nextIndex;
        }
    }
}

void cb_client_processPacket(InputMemoryBitStream& imbs, SocketAddress* fromAddress)
{
    NW_MGR_CLNT->processPacket(imbs, fromAddress);
}

NetworkManagerClient::NetworkManagerClient(std::string serverIPAddress, std::string username, uint16_t port, OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf, RemoveProcessedMovesFunc rpmf, GetMoveListFunc gmlf, OnPlayerWelcomedFunc opwf) :
_packetHandler(INST_REG.get<TimeTracker>(INSK_TIME_CLNT), port, cb_client_processPacket),
_serverAddress(SocketAddressFactory::createIPv4FromString(serverIPAddress)),
_username(username),
_removeProcessedMovesFunc(rpmf),
_getMoveListFunc(gmlf),
_onPlayerWelcomedFunc(opwf),
_entityRegistry(oerf, oedf),
_replicationManagerClient(),
_avgRoundTripTime(INST_REG.get<TimeTracker>(INSK_TIME_CLNT), 1.0f),
_state(NWCS_SAYING_HELLO),
_deliveryNotificationManager(INST_REG.get<TimeTracker>(INSK_TIME_CLNT), true, false),
_timeOfLastHello(0),
_lastMoveProcessedByServerTimestamp(0),
_lastServerCommunicationTimestamp(0),
_isRequestingToAddLocalPlayer(false),
_isRequestingToDropLocalPlayer(0),
_nextIndex(0),
_hasReceivedNewState(false),
_numMovesProcessed(0),
_port(port)
{
    // Empty
}

NetworkManagerClient::~NetworkManagerClient()
{
    OutputMemoryBitStream ombs;
    ombs.write<uint8_t, 4>(static_cast<uint8_t>(NWPT_CLNT_EXIT));
    sendPacket(ombs);
    
    if (SOCKET_UTIL.isLoggingEnabled())
    {
        _deliveryNotificationManager.logStats();
    }
}
