//
//  DanteServer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/24/24.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void cb_dante_server_onEntityRegistered(Entity* e)
{
    DANTE_SERVER->world().addNetworkEntity(e);
    
    if (e->isPlayer() && e->playerInfo()._playerID == 1)
    {
        uint32_t entityLayoutKey = e->networkDataField("entityLayoutKey").valueUInt32();
        EntityLayoutDef& eld = ENTITY_LAYOUT_MGR.entityLayoutDef(entityLayoutKey);
        DANTE_SERVER->populateFromEntityLayout(eld);
    }
}

void cb_dante_server_onEntityDeregistered(Entity* e)
{
    bool needsRestart = false;
    
    if (e->isPlayer())
    {
        uint8_t playerID = e->playerInfo()._playerID;
        needsRestart = NW_SRVR->getClientProxy(playerID) != nullptr;
    }
    
    DANTE_SERVER->world().removeNetworkEntity(e);
    
    if (needsRestart)
    {
        DANTE_SERVER->restart();
    }
}

void cb_dante_server_handleNewClient(std::string playerName, uint8_t playerID)
{
    DANTE_SERVER->handleNewClient(playerName, playerID);
}

void cb_dante_server_handleLostClient(ClientProxy& cp, uint8_t localPlayerIndex)
{
    DANTE_SERVER->handleLostClient(cp, localPlayerIndex);
}

DanteServer* DanteServer::s_instance = nullptr;

void DanteServer::create()
{
    assert(s_instance == nullptr);
    
    s_instance = new DanteServer();
}

DanteServer* DanteServer::getInstance()
{
    return s_instance;
}

void DanteServer::destroy()
{
    assert(s_instance != nullptr);
    
    delete s_instance;
    s_instance = nullptr;
}

void DanteServer::update()
{
    if (!_isConnected || !NW_SRVR->isConnected())
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

void DanteServer::handleNewClient(std::string playerName, uint8_t playerID)
{
    addPlayer(playerName, playerID);
}

void DanteServer::handleLostClient(ClientProxy& cp, uint8_t localPlayerIndex)
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

void DanteServer::populateFromEntityLayout(EntityLayoutDef& eld)
{
    EntityLayoutManagerLoader::loadEntityLayout(eld, _entityIDManager, true);
    
    LOG("populateFromEntityLayout");
    World& w = world();
    std::vector<Entity*> toDelete = w.getDynamicEntities();
    for (Entity* e : toDelete)
    {
        LOG("entity: %d", e->getID());
        NW_SRVR->deregisterEntity(e);
    }
//    for (auto& e : w.getDynamicEntities())
//    {
//        LOG("entity: %d", e->getID());
//        NW_SRVR->deregisterEntity(e);
//    }
    
    w.populateFromEntityLayout(eld);
    
    for (auto& eid : eld._entitiesNetwork)
    {
        NW_SRVR->registerEntity(ENTITY_MGR.createEntity(eid));
    }
}

void DanteServer::restart()
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

World& DanteServer::world()
{
    return *_world;
}

void DanteServer::updateWorld()
{
    world().beginFrame();
    for (Entity* e : world().getPlayers())
    {
        uint8_t playerID = e->playerInfo()._playerID;
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
    
    for (Entity* e : world().getDynamicEntities())
    {
        e->runAI();
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

void DanteServer::handleDirtyStates(std::vector<Entity*>& entities)
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

void DanteServer::addPlayer(std::string playerName, uint8_t playerID)
{
    ClientProxy* cp = NW_SRVR->getClientProxy(playerID);
    assert(cp != nullptr);

    // TODO, assert that we don't already have a player for this ID
    uint32_t spawnX = 18;
    uint32_t spawnY = 9;
    uint32_t entityLayoutKey = 'Z001';

    uint32_t key = 'ROBT';
    uint32_t networkID = _entityIDManager.getNextPlayerEntityID();
    EntityInstanceDef eid(networkID, key, spawnX, spawnY, true);
    Entity* e = ENTITY_MGR.createEntity(eid);
    e->playerInfo()._playerAddressHash = cp->getMachineAddress()->getHash();
    e->playerInfo()._playerName = playerName;
    e->playerInfo()._playerAddress = cp->getMachineAddress()->toString();
    e->playerInfo()._playerID = playerID;
    if (playerID == 1)
    {
        e->networkDataField("entityLayoutKey").setValueUInt32(entityLayoutKey);
    }

    NW_SRVR->registerEntity(e);
}

void DanteServer::removePlayer(uint8_t playerID)
{
    for (Entity* e : world().getPlayers())
    {
        if (e->playerInfo()._playerID == playerID)
        {
            NW_SRVR->deregisterEntity(e);
            return;
        }
    }
}

DanteServer::DanteServer() :
_entityIDManager(),
_timeTracker(),
_world(new Box2DPhysicsWorld()),
_isRestarting(false),
_isConnected(false)
{
    ServerHelper* serverHelper = nullptr;
#if IS_DESKTOP
    if (ENGINE_CFG.useSteamNetworking())
    {
        serverHelper = new SteamServerHelper(ENGINE_CFG.steamGameDir(),
                                             ENGINE_CFG.versionName(),
                                             ENGINE_CFG.steamProductName(),
                                             ENGINE_CFG.steamProductDescription(),
                                             ENGINE_CFG.serverPort(),
                                             _timeTracker,
                                             NW_SRVR_CALLBACKS);
    }
    else
    {
#endif
        serverHelper = new SocketServerHelper(ENGINE_CFG.serverPort(),
                                              _timeTracker,
                                              NW_SRVR_CALLBACKS);
#if IS_DESKTOP
    }
#endif
    
    NetworkServer::create(serverHelper,
                          _entityIDManager,
                          _timeTracker,
                          cb_dante_server_onEntityRegistered,
                          cb_dante_server_onEntityDeregistered,
                          cb_dante_server_handleNewClient,
                          cb_dante_server_handleLostClient);
    assert(NW_SRVR != nullptr);
    
    _isConnected = NW_SRVR->connect();
}

DanteServer::~DanteServer()
{
    NetworkServer::destroy();
    delete _world;
}
