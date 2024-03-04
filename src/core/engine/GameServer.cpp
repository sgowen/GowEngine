//
//  GameServer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/24/24.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void cb_game_server_onEntityRegistered(Entity* e)
{
    GAME_SERVER->world().addNetworkEntity(e);
    
    if (e->isPlayer() && e->playerInfo()._playerID == 1)
    {
        uint32_t entityLayoutKey = e->networkDataField("entityLayoutKey").valueUInt32();
        EntityLayout& eld = ENTITY_LAYOUT_MGR.entityLayout(entityLayoutKey);
        GAME_SERVER->populateFromEntityLayout(eld);
    }
}

void cb_game_server_onEntityDeregistered(Entity* e)
{
    bool needsRestart = false;
    
    if (e->isPlayer())
    {
        uint8_t playerID = e->playerInfo()._playerID;
        needsRestart = NW_SRVR->getClientProxy(playerID) != nullptr;
    }
    
    GAME_SERVER->world().removeNetworkEntity(e);
    
    if (needsRestart)
    {
        GAME_SERVER->restart();
    }
}

void cb_game_server_handleNewClient(std::string playerName, uint8_t playerID)
{
    GAME_SERVER->handleNewClient(playerName, playerID);
}

void cb_game_server_handleLostClient(ClientProxy& cp, uint8_t localPlayerIndex)
{
    GAME_SERVER->handleLostClient(cp, localPlayerIndex);
}

GameServer* GameServer::s_instance = nullptr;

void GameServer::create(Config& config)
{
    assert(s_instance == nullptr);
    
    s_instance = new GameServer(config);
}

GameServer* GameServer::getInstance()
{
    return s_instance;
}

void GameServer::destroy()
{
    assert(s_instance != nullptr);
    
    delete s_instance;
    s_instance = nullptr;
}

void GameServer::update()
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

void GameServer::handleNewClient(std::string playerName, uint8_t playerID)
{
    spawnPlayer(playerName, playerID);
}

void GameServer::handleLostClient(ClientProxy& cp, uint8_t localPlayerIndex)
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

void GameServer::populateFromEntityLayout(EntityLayout& eld)
{
    EntityLayoutManagerLoader::loadEntityLayout(eld, _entityIDManager, true);
    
    World& w = world();
    std::vector<Entity*> toDelete = w.getDynamicEntities();
    for (Entity* e : toDelete)
    {
        NW_SRVR->deregisterEntity(e);
    }
    
    w.populateFromEntityLayout(eld);
    
    for (auto& eid : eld._entitiesNetwork)
    {
        NW_SRVR->registerEntity(ENTITY_MGR.createEntity(eid));
    }
}

void GameServer::restart()
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
        spawnPlayer(cp->getUsername(), pair.first);
    }
}

World& GameServer::world()
{
    return *_world;
}

void GameServer::updateWorld()
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

void GameServer::handleDirtyStates(std::vector<Entity*>& entities)
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

void GameServer::spawnPlayer(std::string playerName, uint8_t playerID)
{
    ClientProxy* cp = NW_SRVR->getClientProxy(playerID);
    assert(cp != nullptr);

    // TODO, assert that we don't already have a player for this ID
    
    uint32_t key = 'PLYR';
    
    EntityDef& ed = ENTITY_MGR.getEntityDef(key);
    uint32_t spawnX = ed._data.getUInt("spawnX", 33);
    uint32_t spawnY = ed._data.getUInt("spawnY", 22);
    
    uint32_t networkID = _entityIDManager.getNextPlayerEntityID();
    EntityInstanceDef eid(networkID, key, spawnX, spawnY, true);
    
    Entity* e = ENTITY_MGR.createEntity(ed, eid);
    e->playerInfo()._playerAddressHash = cp->getMachineAddress()->getHash();
    e->playerInfo()._playerName = playerName;
    e->playerInfo()._playerAddress = cp->getMachineAddress()->toString();
    e->playerInfo()._playerID = playerID;
    if (playerID == 1)
    {
        uint32_t entityLayoutKey = ENTITY_LAYOUT_MGR.getFirstLayout();
        e->networkDataField("entityLayoutKey").setValueUInt32(entityLayoutKey);
    }

    NW_SRVR->registerEntity(e);
}

void GameServer::removePlayer(uint8_t playerID)
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

GameServer::GameServer(Config& config) :
_config(config),
_entityIDManager(),
_timeTracker(),
_world(nullptr),
_isRestarting(false),
_isConnected(false)
{
    std::string physicsEngine = _config.getString("physicsEngine");
    bool isBox2D = physicsEngine == "Box2D";
    
    if (isBox2D)
    {
        _world = new Box2DPhysicsWorld();
    }
    else
    {
        _world = new NosPhysicsWorld();
    }
    
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
                          cb_game_server_onEntityRegistered,
                          cb_game_server_onEntityDeregistered,
                          cb_game_server_handleNewClient,
                          cb_game_server_handleLostClient);
    assert(NW_SRVR != nullptr);
    
    _isConnected = NW_SRVR->connect();
}

GameServer::~GameServer()
{
    NetworkServer::destroy();
    delete _world;
}
