//
//  NetworkClient.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

NetworkClient* NetworkClient::s_instance = nullptr;

void NetworkClient::create(std::string serverIPAddress, std::string username, uint16_t port, TimeTracker& tt, OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf, RemoveProcessedMovesFunc rpmf, OnPlayerWelcomedFunc opwf)
{
    assert(s_instance == nullptr);
    
    s_instance = new NetworkClient(serverIPAddress, username, port, tt, oerf, oedf, rpmf, opwf);
    
    assert(NW_CLNT != nullptr);
}

NetworkClient * NetworkClient::getInstance()
{
    return s_instance;
}

void NetworkClient::destroy()
{
    assert(s_instance != nullptr);
    
    delete s_instance;
    s_instance = nullptr;
}

NetworkClientState NetworkClient::processIncomingPackets()
{
    _hasReceivedNewState = false;
    
    _packetHandler.processIncomingPackets();
    
    uint32_t time = _timeTracker._time;
    uint32_t dcTime = _lastServerCommunicationTimestamp + NW_SRVR_TIMEOUT;
    if (time > dcTime)
    {
        _state = NWCS_DISCONNECTED;
    }
    
    return _state;
}

void NetworkClient::sendOutgoingPackets(MoveList& ml)
{
    switch (_state)
    {
        case NWCS_SAYING_HELLO:
            updateSayingHello();
            break;
        case NWCS_WELCOMED:
            updateSendingInputPacket(ml);
            updateAddLocalPlayerRequest();
            updateDropLocalPlayerRequest();
            break;
        case NWCS_DISCONNECTED:
        default:
            break;
    }
}

void NetworkClient::requestToAddLocalPlayer()
{
    _isRequestingToAddLocalPlayer = true;
    _isRequestingToDropLocalPlayer = 0;
}

void NetworkClient::requestToDropLocalPlayer(uint8_t index)
{
    assert(index >= 1);
    
    _isRequestingToAddLocalPlayer = false;
    _isRequestingToDropLocalPlayer = index;
    _playerIDs.erase(index);
    updateNextIndex();
}

const MovingAverage& NetworkClient::getBytesReceivedPerSecond() const
{
    return _packetHandler.getBytesReceivedPerSecond();
}

const MovingAverage& NetworkClient::getBytesSentPerSecond() const
{
    return _packetHandler.getBytesSentPerSecond();
}

const MovingAverage& NetworkClient::getAvgRoundTripTime() const
{
    return _avgRoundTripTime;
}

float NetworkClient::getRoundTripTime() const
{
    return _avgRoundTripTime.getValue();
}

bool NetworkClient::isPlayerIDLocal(uint8_t playerID) const
{
    for (auto const& entry : _playerIDs)
    {
        if (entry.second == playerID)
        {
            return true;
        }
    }
    
    return false;
}

bool NetworkClient::hasReceivedNewState()
{
    return _hasReceivedNewState;
}

std::map<uint8_t, uint8_t>& NetworkClient::getPlayerIDs()
{
    return _playerIDs;
}

std::string& NetworkClient::getPlayerName()
{
    return _username;
}

NetworkClientState NetworkClient::state() const
{
    return _state;
}

bool NetworkClient::connect()
{
    if (IS_NETWORK_LOGGING_ENABLED())
    {
        LOG("Client Initializing PacketHandler at port %hu", _port);
    }
    
    int error = _packetHandler.connect();
    if (error != NO_ERROR &&
        IS_NETWORK_LOGGING_ENABLED())
    {
        LOG("Client connect failed. Error code %d", error);
    }
    
    return error == NO_ERROR;
}

void NetworkClient::processPacket(InputMemoryBitStream& imbs, SocketAddress* fromAddress)
{
    if (IS_NETWORK_LOGGING_ENABLED())
    {
        LOG("Client processPacket bit length: %d", imbs.getRemainingBitCount());
    }
    
    _lastServerCommunicationTimestamp = _timeTracker._time;
    
    uint8_t packetType;
    imbs.readBits(packetType, 4);
    
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
            if (IS_NETWORK_LOGGING_ENABLED())
            {
                LOG("Server has shut down");
            }
            _state = NWCS_DISCONNECTED;
            break;
        default:
            if (IS_NETWORK_LOGGING_ENABLED())
            {
                LOG("Unknown packet type received from %s", fromAddress->toString().c_str());
            }
            break;
    }
}

void NetworkClient::onMoveProcessed()
{
    ++_numMovesProcessed;
}

uint32_t NetworkClient::getNumMovesProcessed()
{
    return _numMovesProcessed;
}

void NetworkClient::sendPacket(const OutputMemoryBitStream& ombs)
{
    if (IS_NETWORK_LOGGING_ENABLED())
    {
        LOG("Client    sendPacket bit length: %d", ombs.getBitLength());
    }
    
    _packetHandler.sendPacket(ombs, _serverAddress);
}

void NetworkClient::updateSayingHello()
{
    uint32_t time = _timeTracker._time;
    if (time > _timeOfLastHello + NW_CLNT_TIME_BETWEEN_HELLOS)
    {
        // FIXME, player name length (and this packet size)
        // should be configurable
        OutputMemoryBitStream ombs(64);
        ombs.writeBits(static_cast<uint8_t>(NWPT_HELLO), 4);
        ombs.writeSmall(getPlayerName());
        sendPacket(ombs);
        
        _timeOfLastHello = time;
    }
}

void NetworkClient::handleWelcomePacket(InputMemoryBitStream& imbs)
{
    if (_state != NWCS_SAYING_HELLO)
    {
        return;
    }
    
    uint8_t playerID;
    imbs.readBits(playerID, 3);
    
    _state = NWCS_WELCOMED;
    
    _playerIDs.clear();
    _playerIDs[_nextIndex] = playerID;
    
    if (IS_NETWORK_LOGGING_ENABLED())
    {
        LOG("Client '%s' was welcomed as player %d", getPlayerName().c_str(), playerID);
    }
    
    _onPlayerWelcomedFunc(playerID);
    
    updateNextIndex();
}

void NetworkClient::handleLocalPlayerAddedPacket(InputMemoryBitStream& imbs)
{
    if (_state != NWCS_WELCOMED || !_isRequestingToAddLocalPlayer)
    {
        return;
    }
    
    uint8_t playerID;
    imbs.read(playerID);
    
    _playerIDs[_nextIndex] = playerID;
    
    if (IS_NETWORK_LOGGING_ENABLED())
    {
        LOG("'%s(%d)' was welcomed on client as player %d", getPlayerName().c_str(), _nextIndex, playerID);
    }
    
    _isRequestingToAddLocalPlayer = false;
    
    _onPlayerWelcomedFunc(playerID);
    
    updateNextIndex();
}

void NetworkClient::handleLocalPlayerDeniedPacket()
{
    if (IS_NETWORK_LOGGING_ENABLED())
    {
        LOG("'%s(%d)' was denied on client...", getPlayerName().c_str(), _nextIndex);
    }
    
    _isRequestingToAddLocalPlayer = false;
}

void NetworkClient::handleStatePacket(InputMemoryBitStream& imbs)
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
        
        float rtt = _timeTracker.realTime() - _timeTracker.realTime(_lastMoveProcessedByServerTimestamp);
        _avgRoundTripTime.update(rtt);
        
        _removeProcessedMovesFunc(_lastMoveProcessedByServerTimestamp);
    }
    
    _replicationManagerClient.read(imbs, _entityRegistry);
    _hasReceivedNewState = true;
}

void NetworkClient::updateSendingInputPacket(MoveList& ml)
{
    OutputMemoryBitStream ombs(NW_MAX_PACKET_SIZE);
    ombs.writeBits(static_cast<uint8_t>(NWPT_INPUT), 4);
    
    _deliveryNotificationManager.writeState(ombs);
    
    ombs.write(ml.hasMoves());
    if (ml.hasMoves())
    {
        int moveCount = ml.getNumMovesAfterTimestamp(_lastMoveProcessedByServerTimestamp);
        assert(moveCount <= NW_CLNT_MAX_NUM_MOVES);
        ombs.writeBits(moveCount, NBITS(NW_CLNT_MAX_NUM_MOVES));
        
        std::deque<Move>::const_iterator moveItr = ml.begin();
        
        for (int i = 0; i < moveCount; ++i, ++moveItr)
        {
            moveItr->write(ombs);
        }
    }
    
    sendPacket(ombs);
}

void NetworkClient::updateAddLocalPlayerRequest()
{
    if (!_isRequestingToAddLocalPlayer)
    {
        return;
    }
    
    uint32_t time = _timeTracker._time;
    if (time > _timeOfLastHello + NW_CLNT_TIME_BETWEEN_HELLOS)
    {
        OutputMemoryBitStream ombs(2);
        ombs.writeBits(static_cast<uint8_t>(NWPT_ADD_LOCAL_PLAYER), 4);
        ombs.write(_nextIndex);
        sendPacket(ombs);
        
        _timeOfLastHello = time;
    }
}

void NetworkClient::updateDropLocalPlayerRequest()
{
    if (_isRequestingToDropLocalPlayer == 0)
    {
        return;
    }
    
    _isRequestingToAddLocalPlayer = false;
    
    OutputMemoryBitStream ombs(1);
    ombs.writeBits(static_cast<uint8_t>(NWPT_DROP_LOCAL_PLAYER), 4);
    ombs.write(_isRequestingToDropLocalPlayer);
    sendPacket(ombs);
    
    _isRequestingToDropLocalPlayer = 0;
}

void NetworkClient::updateNextIndex()
{
    _nextIndex = 0;
    for (auto const& entry : _playerIDs)
    {
        if (entry.first == _nextIndex)
        {
            ++_nextIndex;
        }
    }
}

void cb_client_processPacket(InputMemoryBitStream& imbs, SocketAddress* fromAddress)
{
    NW_CLNT->processPacket(imbs, fromAddress);
}

NetworkClient::NetworkClient(std::string serverIPAddress, std::string username, uint16_t port, TimeTracker& tt, OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf, RemoveProcessedMovesFunc rpmf, OnPlayerWelcomedFunc opwf) :
_timeTracker(tt),
_packetHandler(_timeTracker, port, cb_client_processPacket),
_serverAddress(SocketAddressFactory::createIPv4FromString(serverIPAddress)),
_username(username),
_removeProcessedMovesFunc(rpmf),
_onPlayerWelcomedFunc(opwf),
_deliveryNotificationManager(_timeTracker, true, false),
_entityRegistry(oerf, oedf),
_replicationManagerClient(),
_state(NWCS_SAYING_HELLO),
_timeOfLastHello(0),
_nextIndex(0),
_lastMoveProcessedByServerTimestamp(0),
_lastServerCommunicationTimestamp(0),
_avgRoundTripTime(_timeTracker, 1.0f),
_isRequestingToAddLocalPlayer(false),
_isRequestingToDropLocalPlayer(0),
_hasReceivedNewState(false),
_numMovesProcessed(0),
_port(port)
{
    // Empty
}

NetworkClient::~NetworkClient()
{
    _entityRegistry.deregisterAll();
    
    OutputMemoryBitStream ombs(1);
    ombs.writeBits(static_cast<uint8_t>(NWPT_CLNT_EXIT), 4);
    sendPacket(ombs);
    
    if (IS_NETWORK_LOGGING_ENABLED())
    {
        _deliveryNotificationManager.logStats();
    }
}
