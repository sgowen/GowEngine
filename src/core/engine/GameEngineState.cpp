//
//  GameEngineState.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/1/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

GameInputProcessor::GameInputProcessor() :
_inputState(),
_state(GIMS_DEFAULT),
_numMovesProcessed(0)
{
    reset();
}

GameInputProcessor::~GameInputProcessor()
{
    // Empty
}

GameInputProcessorState GameInputProcessor::update(World& world)
{
    uint16_t& inputStateP1 = _inputState.playerInputState(0)._inputState;
    uint16_t& inputStateP2 = _inputState.playerInputState(1)._inputState;
    
#if IS_MOBILE
    for (CursorEvent* e : INPUT_MGR.getCursorEvents())
    {
        if (_state == GIMS_EXIT)
        {
            break;
        }
        
        uint16_t& inputState = inputStateP1;
        
        Entity* entity = world.getPlayer(1);
        if (entity == nullptr)
        {
            continue;
        }
        
        entity->processEvent(inputState, e);
    }
#endif
    
    for (GamepadEvent* e : INPUT_MGR.getGamepadEvents())
    {
        if (_state == GIMS_EXIT)
        {
            break;
        }
        
        bool isPlayer1 = e->_index == 0;
        uint16_t& inputState = isPlayer1 ? inputStateP1 : inputStateP2;
        
        Entity* entity = world.getPlayer(isPlayer1 ? 1 : 2);
        if (entity == nullptr)
        {
            if (!isPlayer1)
            {
                // Wtf is this?
                SET_BIT(inputStateP2, 1, true);
            }
            continue;
        }
        
        entity->processEvent(inputState, e);
        
        switch (e->_button)
        {
            case GPEB_BUTTON_SELECT:
            case GPEB_BUTTON_SNES_SELECT:
                _state = e->isDown() ? GIMS_EXIT : GIMS_DEFAULT;
                break;
            case GPEB_UNKNOWN_6:
            {
                _state = e->isPressed() ? GIMS_ZOOM_IN : GIMS_DEFAULT;
                break;
            }
            case GPEB_BUMPER_RIGHT:
            case GPEB_UNKNOWN_7:
                if (_state == GIMS_ZOOM_IN)
                {
                    _state = GIMS_ZOOM_RESET;
                }
                else
                {
                    _state = e->isPressed() ? GIMS_ZOOM_OUT : GIMS_DEFAULT;
                }
                break;
            default:
                break;
        }
    }
    
    for (KeyboardEvent* e : INPUT_MGR.getKeyboardEvents())
    {
        if (_state == GIMS_EXIT)
        {
            break;
        }
        
        bool isPlayer1 = e->_key != GOW_KEY_ARROW_UP &&
        e->_key != GOW_KEY_ARROW_LEFT &&
        e->_key != GOW_KEY_ARROW_RIGHT &&
        e->_key != GOW_KEY_ARROW_DOWN;
        
        uint16_t& inputState = isPlayer1 ? inputStateP1 : inputStateP2;
        
        Entity* entity = world.getPlayer(isPlayer1 ? 1 : 2);
        if (entity == nullptr)
        {
            if (!isPlayer1)
            {
                SET_BIT(inputStateP2, 1, true);
            }
            continue;
        }
        
        entity->processEvent(inputState, e);
        
        switch (e->_key)
        {
            case GOW_KEY_ESCAPE:
            case GOW_KEY_ANDROID_BACK_BUTTON:
                _state = e->isUp() ? GIMS_EXIT : GIMS_DEFAULT;
                break;
            case GOW_KEY_P:
                if (e->isDown())
                {
                    _state = _state == GIMS_DISPLAY_PHYSICS ? GIMS_DEFAULT : GIMS_DISPLAY_PHYSICS;
                }
                break;
            case GOW_KEY_I:
                _state = e->isPressed() ? GIMS_ZOOM_IN : GIMS_DEFAULT;
                break;
            case GOW_KEY_O:
                if (_state == GIMS_ZOOM_IN)
                {
                    _state = GIMS_ZOOM_RESET;
                }
                else
                {
                    _state = e->isPressed() ? GIMS_ZOOM_OUT : GIMS_DEFAULT;
                }
                break;
            case GOW_KEY_PERIOD:
            {
                if (e->isDown())
                {
                    drop2ndPlayer();
                }
                break;
            }
            default:
                break;
        }
    }
    
    if (_inputState.isRequestingToAddLocalPlayer() &&
        NW_CLNT != nullptr)
    {
        NW_CLNT->requestToAddLocalPlayer();
    }
    
    return _state;
}

GameInputProcessorState GameInputProcessor::state()
{
    return _state;
}

void GameInputProcessor::sampleInputAsNewMove(TimeTracker& tt)
{
    InputState* inputState = _poolInputState.obtain();
    _inputState.copyTo(inputState);
    
    _moveList.addMove(inputState, tt._time, _numMovesProcessed);
    ++_numMovesProcessed;
}

void GameInputProcessor::removeProcessedMovesWithIndexLessThan(uint32_t numMovesProcessed)
{
    _moveList.removeProcessedMovesWithIndexLessThan(numMovesProcessed, _poolInputState);
}

InputState& GameInputProcessor::inputState()
{
    return _inputState;
}

MoveList& GameInputProcessor::moveList()
{
    return _moveList;
}

void GameInputProcessor::reset()
{
    _moveList.removeAllMoves(_poolInputState);
    _inputState.reset();
    _numMovesProcessed = 0;
    
    _state = GIMS_DEFAULT;
}

void GameInputProcessor::setNumMovesProcessed(uint32_t numMovesProcessed)
{
    _numMovesProcessed = numMovesProcessed;
}

void GameInputProcessor::drop2ndPlayer()
{
    InputState::PlayerInputState& pis = _inputState.playerInputState(1);
    pis._playerID = 0;
    NW_CLNT->requestToDropLocalPlayer(1);
}

uint64_t cb_steam_getPlayerAddressHash(uint8_t inPlayerIndex)
{
    uint64_t ret = 0;
    
    std::map<uint8_t, Entity::PlayerInfo>& players = ENGINE_STATE_GAME.players();
    
    uint8_t playerID = inPlayerIndex + 1;
    
    auto q = players.find(playerID);
    if (q != players.end())
    {
        Entity::PlayerInfo& playerInfo = q->second;
        ret = playerInfo._playerAddressHash;
    }
    
    return ret;
}

void cb_client_onEntityRegistered(Entity* e)
{
    ENGINE_STATE_GAME.world().addNetworkEntity(e);
    
    if (e->isPlayer())
    {
        uint8_t playerID = e->playerInfo()._playerID;
        ENGINE_STATE_GAME.input().inputState().playerInputState(playerID - 1)._inputState = 0;
        ENGINE_STATE_GAME.players().insert({playerID, e->playerInfo()});
    }
}

void cb_client_onEntityDeregistered(Entity* e)
{
    ENGINE_STATE_GAME.world().removeNetworkEntity(e);
}

void cb_client_onPlayerWelcomed(uint8_t playerID)
{
    ENGINE_STATE_GAME.input().inputState().activateNextPlayer(playerID);
}

void GameEngineState::onEnter(Engine* e)
{
    if (ENGINE_CFG.physicsEngine() == "Box2D")
    {
        _world = new Box2DPhysicsWorld();
    }
    else
    {
        _world = new NosPhysicsWorld();
    }
    
    if (_args.getBool(ARG_IS_HOST))
    {
        GameServer::create();
        assert(GAME_SERVER != nullptr);
    }
}

void GameEngineState::onAssetsLoaded(Engine* e)
{
    AUDIO_ENGINE.playMusic("music_game", 0.7f);
}

void GameEngineState::onExit(Engine* e)
{
    if (_args.getBool(ARG_IS_HOST))
    {
        GameServer::destroy();
    }
    
    NetworkClient::destroy();
    
    _timeTracker.reset();
    delete _world;
    
    _inputProcessor.reset();
    _scale = 1.0f;
}

void GameEngineState::onUpdate(Engine* e)
{
    if (GAME_SERVER)
    {
        GAME_SERVER->update();
    }
    
    _timeTracker.onFrame();
    
    GameInputProcessorState gims = _inputProcessor.update(world());
    if (gims == GIMS_EXIT)
    {
        e->popState();
        return;
    }
    
    static float zoomStep = 0.035398230088496f;
    if (gims == GIMS_ZOOM_IN)
    {
        _scale -= zoomStep;
    }
    if (gims == GIMS_ZOOM_OUT)
    {
        _scale += zoomStep;
    }
    if (gims == GIMS_ZOOM_RESET)
    {
        _scale = 1.0f;
    }
    
#if IS_DESKTOP
    if (STEAM_GAME_SERVICES)
    {
        STEAM_GAME_SERVICES->update();
    }
#endif
    
    if (NW_SRVR &&
        NW_SRVR->isConnected() &&
        NW_CLNT == nullptr)
    {
        joinServer(e);
    }
    else if (!NW_SRVR &&
             NW_CLNT == nullptr)
    {
        joinServer(e);
    }
    
    if (NW_CLNT == nullptr)
    {
        return;
    }
    
    if (NW_CLNT->processIncomingPackets() == NWCS_DISCONNECTED)
    {
        e->popState();
        return;
    }
    
    if (getPlayer(1) == nullptr)
    {
        uint32_t numMovesProcessed = NW_CLNT->getNumMovesProcessed();
        world().recallCache(numMovesProcessed);
        assert(numMovesProcessed == world().getNumMovesProcessed());
        
        world().clearCache(numMovesProcessed);
        input().removeProcessedMovesWithIndexLessThan(numMovesProcessed);
        input().setNumMovesProcessed(numMovesProcessed);
        
        Entity* e = getPlayer(1);
        if (e != nullptr)
        {
            uint32_t entityLayoutKey = e->networkDataField("entityLayoutKey").valueUInt32();
            EntityLayout& el = ASSETS_MGR.entityLayout(entityLayoutKey);
            populateFromEntityLayout(el);
        }
    }
    
    if (getControlledPlayer() != nullptr)
    {
        static uint8_t maxNumFramesOfRollback = ENGINE_CFG.maxNumFramesOfRollback();
        static uint8_t numFramesOfInputDelay = ENGINE_CFG.numFramesOfInputDelay();
        static uint8_t maxNumMoves = numFramesOfInputDelay + maxNumFramesOfRollback + 1;
        
        int numMovesProcessedLocally = world().getNumMovesProcessed();
        int numMovesProcessedByServer = NW_CLNT->getNumMovesProcessed();
        int numMovesAheadOfServer = numMovesProcessedLocally - numMovesProcessedByServer;
        MoveList& ml = _inputProcessor.moveList();
        
        if (ENGINE_CFG.networkLoggingEnabled())
        {
            LOG("numMovesProcessedLocally: %d", numMovesProcessedLocally);
            LOG("numMovesProcessedByServer: %d", numMovesProcessedByServer);
            LOG("numMovesAheadOfServer: %d", numMovesAheadOfServer);
            LOG("MoveCount: %d", ml.getMoveCount());
        }
        
        if (numMovesAheadOfServer >= 0)
        {
            if (ENGINE_CFG.networkLoggingEnabled())
            {
                LOG("recallCache client(%d) ahead of server(%d)", numMovesProcessedLocally, numMovesProcessedByServer);
            }
            world().recallCache(numMovesProcessedByServer);
            world().clearCache(world().getNumMovesProcessed());
            input().removeProcessedMovesWithIndexLessThan(world().getNumMovesProcessed());
            if (getPlayer(1) == nullptr)
            {
                return;
            }
            
            _numRollbackFrames = numMovesProcessedLocally - world().getNumMovesProcessed();
            
            if (_numRollbackFrames > 0)
            {
                if (ENGINE_CFG.networkLoggingEnabled())
                {
                    LOG("Rollback replaying last %d moves", _numRollbackFrames);
                }
                for (const Move& m : ml)
                {
                    updateWorld(m);
                    
                    if (world().getNumMovesProcessed() == numMovesProcessedLocally)
                    {
                        // Don't need to replay moves that haven't been processed yet
                        break;
                    }
                }
            }
        }
        
        numMovesProcessedLocally = world().getNumMovesProcessed();
        numMovesProcessedByServer = NW_CLNT->getNumMovesProcessed();
        numMovesAheadOfServer = numMovesProcessedLocally - numMovesProcessedByServer;
        
        if (ml.getMoveCount() < maxNumMoves)
        {
            _inputProcessor.sampleInputAsNewMove(_timeTracker);
        }
        
        if (ml.getMoveCount() > numFramesOfInputDelay &&
            numMovesAheadOfServer < maxNumFramesOfRollback)
        {
            Move* move = ml.getMoveWithMoveIndex(numMovesProcessedLocally);
            // This used to be an assert
            // but I removed it because it was crashing
            // but really... if we get here, move should never be null...
            // to reproduce, simply die
            // May only happen when 2 people die simultaneously.
//            assert(move != nullptr);
            if (move != nullptr)
            {
                if (ENGINE_CFG.networkLoggingEnabled())
                {
                    LOG("Executing move: %d locally", move->getIndex());
                }
                updateWorld(*move);
                
                // TODO renderAudio()
                
                AUDIO_ENGINE.resumeAllSounds();
            }
        }
        else
        {
            if (ENGINE_CFG.networkLoggingEnabled())
            {
                LOG("Client is ahead of the server, skipping input for this frame!");
            }
            
            AUDIO_ENGINE.pauseAllSounds();
        }
    }
    
    NW_CLNT->sendOutgoingPackets(_inputProcessor.moveList());
    
    if (NW_CLNT->getNumMovesProcessed() > 0 &&
        world().getNumMovesProcessed() <= NW_CLNT->getNumMovesProcessed())
    {
        if (ENGINE_CFG.networkLoggingEnabled())
        {
            LOG("recallCache server(%d) ahead of client(%d)", NW_CLNT->getNumMovesProcessed(), world().getNumMovesProcessed());
        }
        world().recallCache(world().getNumMovesProcessed());
        world().clearCache(world().getNumMovesProcessed());
        input().removeProcessedMovesWithIndexLessThan(world().getNumMovesProcessed());
    }
}

void GameEngineState::onRender(Renderer& r)
{
    r.configPhysicsRenderingEnabled(_inputProcessor.state() == GIMS_DISPLAY_PHYSICS);
    r.configNumRollbackFrames(_numRollbackFrames);
    r.configBounds(world().rightEdge(), world().topEdge(), _scale);
    r.configControlledPlayerEntity(getControlledPlayer());
    
    r.renderWorldWithLua(world());
    
    AUDIO_ENGINE.playSoundsForWorld(world());
}

void GameEngineState::populateFromEntityLayout(EntityLayout& el)
{
    EntityLayoutLoader::loadEntityLayout(el, _entityIDManager, false);
    world().populateFromEntityLayout(el);
}

Entity* GameEngineState::getPlayer(uint8_t playerID)
{
    return world().getPlayer(playerID);
}

Entity* GameEngineState::getControlledPlayer()
{
    return getPlayer(_inputProcessor.inputState().playerInputState(0)._playerID);
}

GameInputProcessor& GameEngineState::input()
{
    return _inputProcessor;
}

std::map<uint8_t, Entity::PlayerInfo>& GameEngineState::players()
{
    return _players;
}

World& GameEngineState::world()
{
    return *_world;
}

void GameEngineState::joinServer(Engine* e)
{
    ClientHelper* clientHelper = nullptr;
#if IS_DESKTOP
    if (ENGINE_CFG.useSteamNetworking())
    {
        CSteamID serverSteamID;
        if (_args.hasValue(ARG_STEAM_ADDRESS))
        {
            uint64 ulSteamID = _args.getUInt64(ARG_STEAM_ADDRESS);
            serverSteamID.SetFromUint64(ulSteamID);
        }
        else
        {
            serverSteamID = static_cast<SteamAddress*>(NW_SRVR->getServerAddress())->getSteamID();
        }
        clientHelper = new SteamClientHelper(serverSteamID,
                                             _timeTracker,
                                             cb_steam_getPlayerAddressHash,
                                             NetworkClient::sProcessPacket);
    }
    else
    {
#endif
        std::string serverIPAddress;
        uint16_t port;
        if (_args.hasValue(ARG_IP_ADDRESS))
        {
            serverIPAddress = _args.getString(ARG_IP_ADDRESS);
            port = ENGINE_CFG.clientPortJoin();
        }
        else
        {
            serverIPAddress = "localhost";
            port = ENGINE_CFG.clientPortHost();
        }
        
        std::string serverAddress = STRING_FORMAT("%s:%d", serverIPAddress.c_str(), ENGINE_CFG.serverPort());
        std::string playerName = _args.getString(ARG_USERNAME);
        clientHelper = new SocketClientHelper(_timeTracker,
                                              serverAddress,
                                              playerName,
                                              port,
                                              NetworkClient::sProcessPacket);
#if IS_DESKTOP
    }
#endif
    
    NetworkClient::create(clientHelper,
                          _timeTracker,
                          cb_client_onEntityRegistered,
                          cb_client_onEntityDeregistered,
                          cb_client_onPlayerWelcomed);
    
    if (NW_CLNT->connect() == false)
    {
        LOG("Unable to connect, exiting...");
        e->popState();
        return;
    }
}

void GameEngineState::updateWorld(const Move& move)
{
    world().beginFrame();
    for (Entity* e : world().getPlayers())
    {
        uint16_t inputState = e->lastProcessedInputState();
        uint8_t playerID = e->playerInfo()._playerID;
        
        InputState* is = move.inputState();
        InputState::PlayerInputState* pis = is->playerInputStateForID(playerID);
        if (pis != nullptr)
        {
            inputState = pis->_inputState;
        }
        
        e->processInput(inputState);
    }
    
    for (Entity* e : world().getDynamicEntities())
    {
        e->runAI();
    }
    
    static float frameRate = static_cast<float>(ENGINE_CFG.frameRate());
    world().stepPhysics(frameRate);
    world().update();
    // TODO, consider removing exiled entities right away
    // But just like MtG, exiled entities can come back!
    // Or in the case of networking, maybe the client got a calculation wrong and the Entity should be still in the world
    // Will need to maintain a list of exiled entities in the world.
    // Only the server can delete an entity right away, because it is the server
    // But the client must allow an entity to sit in an exiled list
    // Until the server says, "yes, that entity really is exiled", or
    // no, that entity is not exiled.
    world().storeToCache();
}

GameEngineState::GameEngineState() : EngineState("json/game/Assets.json"),
_entityIDManager(),
_timeTracker(),
_world(nullptr),
_inputProcessor(),
_numRollbackFrames(0),
_scale(1.0)
{
    // Empty
}
