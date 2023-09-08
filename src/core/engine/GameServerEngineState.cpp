//
//  GameServerEngineState.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/27/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void cb_server_onEntityRegistered(Entity* e)
{
    ENGINE_STATE_GAME_SRVR.world().addNetworkEntity(e);
    
    if (e->isPlayer() && e->networkDataField("playerID").valueUInt8() == 1)
    {
        uint32_t entityLayoutKey = e->networkDataField("entityLayoutKey").valueUInt32();
        EntityLayoutDef& eld = ENTITY_LAYOUT_MGR.entityLayoutDef(entityLayoutKey);
        ENGINE_STATE_GAME_SRVR.populateFromEntityLayout(eld);
    }
}

void cb_server_onEntityDeregistered(Entity* e)
{
    bool needsRestart = false;
    
    if (e->isPlayer())
    {
        uint8_t playerID = e->networkDataField("playerID").valueUInt8();
        needsRestart = NW_SRVR->getClientProxy(playerID) != nullptr;
    }
    
    ENGINE_STATE_GAME_SRVR.world().removeNetworkEntity(e);
    
    if (needsRestart)
    {
        ENGINE_STATE_GAME_SRVR.restart();
    }
}

void cb_server_handleNewClient(std::string username, uint8_t playerID)
{
    ENGINE_STATE_GAME_SRVR.handleNewClient(username, playerID);
}

void cb_server_handleLostClient(ClientProxy& cp, uint8_t localPlayerIndex)
{
    ENGINE_STATE_GAME_SRVR.handleLostClient(cp, localPlayerIndex);
}

void GameServerEngineState::enter(Engine* e)
{
    NetworkServer::create(ENGINE_CFG.serverPort(),
                          _entityIDManager,
                          _timeTracker,
                          cb_server_onEntityRegistered,
                          cb_server_onEntityDeregistered,
                          cb_server_handleNewClient,
                          cb_server_handleLostClient);
    
    _isConnected = NW_SRVR->connect();
}

void GameServerEngineState::execute(Engine* e)
{
    switch (e->requestedStateAction())
    {
        case ERSA_UPDATE:
            update(e);
            break;
        default:
            break;
    }
}

void GameServerEngineState::exit(Engine* e)
{
    NetworkServer::destroy();
    _timeTracker.reset();
    world().reset();
    _isRestarting = false;
}

void GameServerEngineState::handleNewClient(std::string username, uint8_t playerID)
{
    addPlayer(username, playerID);
}

void GameServerEngineState::handleLostClient(ClientProxy& cp, uint8_t localPlayerIndex)
{
    if (localPlayerIndex >= 1)
    {
        uint8_t playerID = cp.getPlayerID(localPlayerIndex);
        removePlayer(playerID);
    }
    else
    {
        for (int i = 0; i < cp.getNumPlayers(); ++i)
        {
            uint8_t playerID = cp.getPlayerID(i);
            removePlayer(playerID);
        }
    }
}

void GameServerEngineState::populateFromEntityLayout(EntityLayoutDef& eld)
{
    EntityLayoutManagerLoader::loadEntityLayout(eld, _entityIDManager, true);
    
    for (auto& e : world().getDynamicEntities())
    {
        NW_SRVR->deregisterEntity(e);
    }
    
    world().populateFromEntityLayout(eld);
    
    for (auto& eid : eld._entitiesNetwork)
    {
        NW_SRVR->registerEntity(ENTITY_MGR.createEntity(eid));
    }
}

void GameServerEngineState::restart()
{
    if (_isRestarting)
    {
        return;
    }
    
    _isRestarting = true;
    std::vector<Entity*>& players = world().getPlayers();
    while (!players.empty())
    {
        Entity* e = players.front();
        NW_SRVR->deregisterEntity(e);
    }
    _isRestarting = false;
    
    const std::map<int, ClientProxy*>& playerIDToClientMap = NW_SRVR->playerIDToClientMap();
    for (auto& pair : playerIDToClientMap)
    {
        ClientProxy* cp = pair.second;
        assert(cp != nullptr);
        addPlayer(cp->getUsername(), pair.first);
    }
}

World& GameServerEngineState::world()
{
    return *_world;
}

void GameServerEngineState::update(Engine* e)
{
    if (!_isConnected)
    {
        return;
    }
    
    _timeTracker.onFrame();
    
    int moveCount = NW_SRVR->processIncomingPackets();
    for (int i = 0; i < moveCount; ++i)
    {
        updateWorld();
    }
    NW_SRVR->removeProcessedMoves();
    NW_SRVR->setNumMovesProcessed(world().getNumMovesProcessed());
    NW_SRVR->sendOutgoingPackets();
}

void GameServerEngineState::updateWorld()
{
    world().beginFrame();
    for (Entity* e : world().getPlayers())
    {
        uint8_t playerID = e->networkDataField("playerID").valueUInt8();
        ClientProxy* cp = NW_SRVR->getClientProxy(playerID);
        assert(cp != nullptr);
        
        MoveList& ml = cp->getUnprocessedMoveList();
        Move* m = ml.getMoveWithMoveIndex(world().getNumMovesProcessed());
        assert(m != nullptr);
        
        InputState* is = m->inputState();
        InputState::PlayerInputState* pis = is->playerInputStateForID(playerID);
        if (ml.isMoveProcessed(m))
        {
            // Move will be already processed
            // if player 1 and player 2
            // are from the same ClientProxy
            // In which case, we may not have input
            // for player 2 until a few frames later
            if (pis == nullptr)
            {
                continue;
            }
        }
        
        assert(pis != nullptr);
        
        e->processInput(pis->_inputState);
        ml.markMoveAsProcessed(m);
        cp->setLastMoveTimestampDirty(true);
    }
    
    static float frameRate = static_cast<float>(ENGINE_CFG.frameRate());
    world().stepPhysics(frameRate);
    std::vector<Entity*> toDelete = world().update();
    for (Entity* e : toDelete)
    {
        bool exitImmediately = e->isPlayer();
        NW_SRVR->deregisterEntity(e);
        if (exitImmediately)
        {
            return;
        }
    }
    
    handleDirtyStates(world().getPlayers());
    handleDirtyStates(world().getDynamicEntities());
}

void GameServerEngineState::handleDirtyStates(std::vector<Entity*>& entities)
{
    for (Entity* e : entities)
    {
        uint8_t dirtyState = e->networkController()->refreshDirtyState();
        if (dirtyState > 0)
        {
            NW_SRVR->setStateDirty(e->getID(), dirtyState);
        }
    }
}

void GameServerEngineState::addPlayer(std::string username, uint8_t playerID)
{
    ClientProxy* cp = NW_SRVR->getClientProxy(playerID);
    assert(cp != nullptr);

    uint32_t spawnX = playerID == 1 ? 64 : 80;
    uint32_t spawnY = 128;

    uint32_t key = playerID == 1 ? 'JON0' : 'JON0';
    uint32_t networkID = _entityIDManager.getNextPlayerEntityID();
    EntityInstanceDef eid(networkID, key, spawnX, spawnY, true);
    Entity* e = ENTITY_MGR.createEntity(eid);
    e->networkDataField("username").setValueString(username);
    e->networkDataField("userAddress").setValueString(cp->getSocketAddress()->toString());
    e->networkDataField("playerID").setValueUInt8(playerID);
    if (playerID == 1)
    {
        e->networkDataField("entityLayoutKey").setValueUInt32('L001');
    }

    NW_SRVR->registerEntity(e);
}

void GameServerEngineState::removePlayer(uint8_t playerID)
{
    for (Entity* e : world().getPlayers())
    {
        if (e->networkDataField("playerID").valueUInt8() == playerID)
        {
            NW_SRVR->deregisterEntity(e);
            return;
        }
    }
}

GameServerEngineState::GameServerEngineState() : State<Engine>(),
_entityIDManager(),
_timeTracker(),
_world(new NosPhysicsWorld()),
_isRestarting(false),
_isConnected(false)
{
    // Empty
}
