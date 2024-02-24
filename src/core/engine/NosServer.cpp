//
//  NosServer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/27/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void cb_server_onEntityRegistered(Entity* e)
{
    NOS_SERVER->world().addNetworkEntity(e);
    
    if (e->isPlayer() && e->playerInfo()._playerID == 1)
    {
        uint32_t entityLayoutKey = e->networkDataField("entityLayoutKey").valueUInt32();
        EntityLayoutDef& eld = ENTITY_LAYOUT_MGR.entityLayoutDef(entityLayoutKey);
        NOS_SERVER->populateFromEntityLayout(eld);
    }
}

void cb_server_onEntityDeregistered(Entity* e)
{
    bool needsRestart = false;
    
    if (e->isPlayer())
    {
        uint8_t playerID = e->playerInfo()._playerID;
        needsRestart = NW_SRVR->getClientProxy(playerID) != nullptr;
    }
    
    NOS_SERVER->world().removeNetworkEntity(e);
    
    if (needsRestart)
    {
        NOS_SERVER->restart();
    }
}

void cb_server_handleNewClient(std::string playerName, uint8_t playerID)
{
    NOS_SERVER->handleNewClient(playerName, playerID);
}

void cb_server_handleLostClient(ClientProxy& cp, uint8_t localPlayerIndex)
{
    NOS_SERVER->handleLostClient(cp, localPlayerIndex);
}

NosServer* NosServer::s_instance = nullptr;

void NosServer::create()
{
    assert(s_instance == nullptr);
    
    s_instance = new NosServer();
}

NosServer* NosServer::getInstance()
{
    return s_instance;
}

void NosServer::destroy()
{
    assert(s_instance != nullptr);
    
    delete s_instance;
    s_instance = nullptr;
}

void NosServer::update()
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

void NosServer::handleNewClient(std::string playerName, uint8_t playerID)
{
    addPlayer(playerName, playerID);
}

void NosServer::handleLostClient(ClientProxy& cp, uint8_t localPlayerIndex)
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

void NosServer::populateFromEntityLayout(EntityLayoutDef& eld)
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

void NosServer::restart()
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

World& NosServer::world()
{
    return *_world;
}

void NosServer::updateWorld()
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

void NosServer::handleDirtyStates(std::vector<Entity*>& entities)
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

void NosServer::addPlayer(std::string playerName, uint8_t playerID)
{
    ClientProxy* cp = NW_SRVR->getClientProxy(playerID);
    assert(cp != nullptr);

    uint32_t spawnX = playerID == 1 ? 64 : 80;
    uint32_t spawnY = 128;

    uint32_t key = playerID == 1 ? 'JON0' : 'JON0';
    uint32_t networkID = _entityIDManager.getNextPlayerEntityID();
    EntityInstanceDef eid(networkID, key, spawnX, spawnY, true);
    Entity* e = ENTITY_MGR.createEntity(eid);
    e->playerInfo()._playerAddressHash = cp->getMachineAddress()->getHash();
    e->playerInfo()._playerName = playerName;
    e->playerInfo()._playerAddress = cp->getMachineAddress()->toString();
    e->playerInfo()._playerID = playerID;
    if (playerID == 1)
    {
        e->networkDataField("entityLayoutKey").setValueUInt32('L001');
    }

    NW_SRVR->registerEntity(e);
}

void NosServer::removePlayer(uint8_t playerID)
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

NosServer::NosServer() :
_entityIDManager(),
_timeTracker(),
_world(new NosPhysicsWorld()),
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
                          cb_server_onEntityRegistered,
                          cb_server_onEntityDeregistered,
                          cb_server_handleNewClient,
                          cb_server_handleLostClient);
    assert(NW_SRVR != nullptr);
    
    _isConnected = NW_SRVR->connect();
}

NosServer::~NosServer()
{
    NetworkServer::destroy();
    delete _world;
}
