//
//  DanteGameEngineState.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/24/24.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

DanteGameInputProcessor::DanteGameInputProcessor() :
_inputState(),
_state(DGIMS_DEFAULT),
_numMovesProcessed(0)
{
    reset();
}

DanteGameInputProcessor::~DanteGameInputProcessor()
{
    // Empty
}

DanteGameInputProcessorState DanteGameInputProcessor::update()
{
    uint16_t& inputStateP1 = _inputState.playerInputState(0)._inputState;
    uint16_t& inputStateP2 = _inputState.playerInputState(1)._inputState;
    
#if IS_MOBILE
    Matrix* m = INPUT_MGR.matrix();
    float halfWidth = m->_desc.width() / 2;
    float halfHeight = m->_desc.height() / 2;
    for (CursorEvent* e : INPUT_MGR.getCursorEvents())
    {
        uint16_t& inputState = inputStateP1;
        
        Vector2& pos = INPUT_MGR.convert(e);
        
        SET_BIT(inputState, RISF_MOVING_LEFT, e->isPressed() && pos._x < halfWidth);
        SET_BIT(inputState, RISF_MOVING_RIGHT, e->isPressed() && pos._x > halfWidth);
        
        SET_BIT(inputState, RISF_JUMPING, e->isPressed() && pos._y > halfHeight);
    }
#endif
    
    for (GamepadEvent* e : INPUT_MGR.getGamepadEvents())
    {
        if (_state == DGIMS_EXIT)
        {
            break;
        }
        
        uint16_t& inputState = e->_index == 0 ? inputStateP1 : inputStateP2;
        
        switch (e->_button)
        {
            case GPEB_BUTTON_SELECT:
            case GPEB_BUTTON_SNES_SELECT:
                _state = e->isDown() ? DGIMS_EXIT : DGIMS_DEFAULT;
                break;
            case GPEB_BUTTON_A:
                SET_BIT(inputState, RISF_EXECUTING_ATTACK, e->isPressed());
                break;
            case GPEB_BUTTON_B:
                SET_BIT(inputState, RISF_JUMPING, e->isPressed());
                break;
            case GPEB_D_PAD_LEFT:
            {
                SET_BIT(inputState, RISF_MOVING_LEFT, e->isPressed());
                break;
            }
            case GPEB_D_PAD_RIGHT:
            {
                SET_BIT(inputState, RISF_MOVING_RIGHT, e->isPressed());
                break;
            }
            case GPEB_STICK_LEFT:
            {
                SET_BIT(inputState, RISF_MOVING_LEFT, e->_x < 0);
                SET_BIT(inputState, RISF_MOVING_RIGHT, e->_x > 0);
                break;
            }
            case GPEB_UNKNOWN_6:
            {
                _state = e->isPressed() ? DGIMS_ZOOM_IN : DGIMS_DEFAULT;
                break;
            }
            case GPEB_BUMPER_RIGHT:
            case GPEB_UNKNOWN_7:
                if (_state == DGIMS_ZOOM_IN)
                {
                    _state = DGIMS_ZOOM_RESET;
                }
                else
                {
                    _state = e->isPressed() ? DGIMS_ZOOM_OUT : DGIMS_DEFAULT;
                }
                break;
            default:
                break;
        }
    }
    
    for (KeyboardEvent* e : INPUT_MGR.getKeyboardEvents())
    {
        switch (e->_key)
        {
            case GOW_KEY_ESCAPE:
            case GOW_KEY_ANDROID_BACK_BUTTON:
                _state = e->isUp() ? DGIMS_EXIT : DGIMS_DEFAULT;
                break;
            case GOW_KEY_P:
                if (e->isDown())
                {
                    _state = _state == DGIMS_DISPLAY_PHYSICS ? DGIMS_DEFAULT : DGIMS_DISPLAY_PHYSICS;
                }
                break;
            case GOW_KEY_J:
                SET_BIT(inputStateP1, RISF_JUMPING, e->isPressed());
                break;
            case GOW_KEY_K:
                SET_BIT(inputStateP1, RISF_EXECUTING_ATTACK, e->isPressed());
                break;
            case GOW_KEY_I:
                _state = e->isPressed() ? DGIMS_ZOOM_IN : DGIMS_DEFAULT;
                break;
            case GOW_KEY_O:
                if (_state == DGIMS_ZOOM_IN)
                {
                    _state = DGIMS_ZOOM_RESET;
                }
                else
                {
                    _state = e->isPressed() ? DGIMS_ZOOM_OUT : DGIMS_DEFAULT;
                }
                break;
            case GOW_KEY_A:
                SET_BIT(inputStateP1, RISF_MOVING_LEFT, e->isPressed());
                break;
            case GOW_KEY_D:
                SET_BIT(inputStateP1, RISF_MOVING_RIGHT, e->isPressed());
                break;
            case GOW_KEY_ARROW_UP:
                SET_BIT(inputStateP2, RISF_JUMPING, e->isPressed());
                break;
            case GOW_KEY_ARROW_LEFT:
                SET_BIT(inputStateP2, RISF_MOVING_LEFT, e->isPressed());
                break;
            case GOW_KEY_ARROW_RIGHT:
                SET_BIT(inputStateP2, RISF_MOVING_RIGHT, e->isPressed());
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

DanteGameInputProcessorState DanteGameInputProcessor::state()
{
    return _state;
}

void DanteGameInputProcessor::sampleInputAsNewMove(TimeTracker& tt)
{
    InputState* inputState = _poolInputState.obtain();
    _inputState.copyTo(inputState);
    
    _moveList.addMove(inputState, tt._time, _numMovesProcessed);
    ++_numMovesProcessed;
}

void DanteGameInputProcessor::removeProcessedMovesWithIndexLessThan(uint32_t numMovesProcessed)
{
    _moveList.removeProcessedMovesWithIndexLessThan(numMovesProcessed, _poolInputState);
}

InputState& DanteGameInputProcessor::inputState()
{
    return _inputState;
}

MoveList& DanteGameInputProcessor::moveList()
{
    return _moveList;
}

void DanteGameInputProcessor::reset()
{
    _moveList.removeAllMoves(_poolInputState);
    _inputState.reset();
    _numMovesProcessed = 0;
    
    _state = DGIMS_DEFAULT;
}

void DanteGameInputProcessor::setNumMovesProcessed(uint32_t numMovesProcessed)
{
    _numMovesProcessed = numMovesProcessed;
}

void DanteGameInputProcessor::drop2ndPlayer()
{
    InputState::PlayerInputState& pis = _inputState.playerInputState(1);
    pis._playerID = 0;
    NW_CLNT->requestToDropLocalPlayer(1);
}

uint64_t cb_dante_steam_getPlayerAddressHash(uint8_t inPlayerIndex)
{
    uint64_t ret = 0;
    
    std::map<uint8_t, Entity::PlayerInfo>& players = ENGINE_STATE_GAME_DANTE.players();
    
    uint8_t playerID = inPlayerIndex + 1;
    
    auto q = players.find(playerID);
    if (q != players.end())
    {
        Entity::PlayerInfo& playerInfo = q->second;
        ret = playerInfo._playerAddressHash;
    }
    
    return ret;
}

void cb_dante_client_onEntityRegistered(Entity* e)
{
    ENGINE_STATE_GAME_DANTE.world().addNetworkEntity(e);
    
    if (e->isPlayer())
    {
        ENGINE_STATE_GAME_DANTE.players().insert({e->playerInfo()._playerID, e->playerInfo()});
    }
}

void cb_dante_client_onEntityDeregistered(Entity* e)
{
    ENGINE_STATE_GAME_DANTE.world().removeNetworkEntity(e);
}

void cb_dante_client_onPlayerWelcomed(uint8_t playerID)
{
    ENGINE_STATE_GAME_DANTE.input().inputState().activateNextPlayer(playerID);
}

void DanteGameEngineState::onEnter(Engine* e)
{
    _world = new Box2DPhysicsWorld();
    
    if (_args.getBool(ARG_IS_HOST, false) == true)
    {
        DanteServer::create();
        assert(DANTE_SERVER != nullptr);
    }
}

void DanteGameEngineState::onAssetsLoaded(Engine* e)
{
    std::string filePathEntityManager = _config.getString("filePathEntityManager");
    EntityManagerLoader::initWithJSONFile(ENTITY_MGR, filePathEntityManager);
    
    std::string filePathEntityLayoutManager = _config.getString("filePathEntityLayoutManager");
    EntityLayoutManagerLoader::initWithJSONFile(ENTITY_LAYOUT_MGR, filePathEntityLayoutManager);
    
    if (!ENGINE_CFG.musicDisabled())
    {
        AUDIO_ENGINE.playSound("music_game", 0, 0.7f, true);
    }
}

void DanteGameEngineState::onExit(Engine* e)
{
    if (_args.getBool(ARG_IS_HOST, false) == true)
    {
        DanteServer::destroy();
    }
    
    NetworkClient::destroy();
    
    _timeTracker.reset();
    delete _world;
    
    _inputProcessor.reset();
    _scale = 1.0f;
}

void DanteGameEngineState::onUpdate(Engine* e)
{
    if (DANTE_SERVER)
    {
        DANTE_SERVER->update();
    }
    
    _timeTracker.onFrame();
    
    DanteGameInputProcessorState gims = _inputProcessor.update();
    if (gims == DGIMS_EXIT)
    {
        e->popState();
        return;
    }
    
    static float zoomStep = 0.035398230088496f;
    if (gims == DGIMS_ZOOM_IN)
    {
        _scale -= zoomStep;
    }
    if (gims == DGIMS_ZOOM_OUT)
    {
        _scale += zoomStep;
    }
    if (gims == DGIMS_ZOOM_RESET)
    {
        _scale = 1.0f;
    }
    
    updateWithNetwork(e);
}

void DanteGameEngineState::onRender(Renderer& r, double extrapolation)
{
    const float baseRight = r.matrix()._base._right;
    const float baseTop = r.matrix()._base._top;
    float rightEdge = _world->rightEdge();
    float topEdge = _world->topEdge();
    float maxWidthScale = rightEdge / baseRight;
    float maxHeightScale = topEdge / baseTop;
    float maxScale = CLAMP(GOW_MIN(maxWidthScale, maxHeightScale), 1.0f, 200.0f);
    _scale = CLAMP(_scale, 0.125f, maxScale);
    
    // TODO, in the proceeding rendering code, consider using multiple matrices instead of changing
    // the "main" one over and over again
    
    r.bindFramebuffer("behindPlayer");
    
    Entity* controlledPlayer = getControlledPlayer();
    
    r.updateMatrixCenteredOnEntityForParallaxLayer(controlledPlayer, 0.6f,  _world->rightEdge(), _world->topEdge(), _scale);
    r.renderEntitiesBoundToTexture(world().getLayers(), "texture_001", "sb_001");
    
    r.updateMatrixCenteredOnEntityForParallaxLayer(controlledPlayer, 0.8f,  _world->rightEdge(), _world->topEdge(), _scale);
    r.renderEntitiesBoundToTexture(world().getLayers(), "texture_002", "sb_002");
    
    r.updateMatrixCenteredOnEntityForParallaxLayer(controlledPlayer, 0.9f,  _world->rightEdge(), _world->topEdge(), _scale);
    r.renderEntitiesBoundToTexture(world().getLayers(), "texture_003", "sb_003");
    
    r.updateMatrixCenteredOnEntity(controlledPlayer, _world->rightEdge(), _world->topEdge(), _scale);
    
    r.renderEntitiesBoundToTexture(world().getLayers(), "texture_004", "sb_004");
    r.renderEntitiesBoundToTexture(world().getDynamicEntities(), "texture_005", "sb_005");
    
    r.bindFramebuffer("behindPlayerNormals");
    
    r.updateMatrixCenteredOnEntityForParallaxLayer(controlledPlayer, 0.6f,  _world->rightEdge(), _world->topEdge(), _scale);
    r.spriteBatcherEnd("n_texture_001", "main", "sprite", "sb_001");
    
    r.updateMatrixCenteredOnEntityForParallaxLayer(controlledPlayer, 0.8f,  _world->rightEdge(), _world->topEdge(), _scale);
    r.spriteBatcherEnd("n_texture_002", "main", "sprite", "sb_002");
    
    r.updateMatrixCenteredOnEntityForParallaxLayer(controlledPlayer, 0.9f,  _world->rightEdge(), _world->topEdge(), _scale);
    r.spriteBatcherEnd("n_texture_003", "main", "sprite", "sb_003");
    
    r.updateMatrixCenteredOnEntity(controlledPlayer, _world->rightEdge(), _world->topEdge(), _scale);
    
    r.spriteBatcherEnd("n_texture_004", "main", "sprite", "sb_004");
    r.spriteBatcherEnd("n_texture_005", "main", "sprite", "sb_005");
    
    r.bindFramebuffer("player");
    r.renderEntitiesBoundToTexture(world().getPlayers(), "texture_006", "sb_006");
    
    r.bindFramebuffer("playerNormals");
    r.spriteBatcherEnd("n_texture_006", "main", "sprite", "sb_006");
    
    r.bindFramebuffer("inFrontOfPlayer");
    r.renderEntitiesBoundToTexture(world().getDynamicEntities(), "texture_007", "sb_007");
    r.renderEntitiesBoundToTexture(world().getDynamicEntities(), "texture_008", "sb_008");
    r.renderEntitiesBoundToTexture(world().getStaticEntities(), "texture_009", "sb_009");
    
    r.bindFramebuffer("inFrontOfPlayerNormals");
    r.spriteBatcherEnd("n_texture_007", "main", "sprite", "sb_007");
    r.spriteBatcherEnd("n_texture_008", "main", "sprite", "sb_008");
    r.spriteBatcherEnd("n_texture_009", "main", "sprite", "sb_009");
    
    r.bindFramebuffer("behindPlayerLights");
    r.renderLight("behindPlayer", "behindPlayerNormals", 0.15f, world().getPlayers());
    
    r.bindFramebuffer("playerLights");
    r.renderLight("player", "playerNormals", 0.1f, world().getPlayers());
    
    r.bindFramebuffer("inFrontOfPlayerLights");
    r.renderLight("inFrontOfPlayer", "inFrontOfPlayerNormals", 0.05f, world().getPlayers());
    
    r.bindFramebuffer("main");
    r.renderFramebuffer("behindPlayerLights");
    r.renderFramebuffer("playerLights");
    r.renderFramebuffer("inFrontOfPlayerLights");
    renderWithNetwork(r);
    
    if (_inputProcessor.state() == DGIMS_DISPLAY_PHYSICS)
    {
        r.renderBox2DPhysics(static_cast<Box2DPhysicsWorld*>(_world));
    }
    
    r.setText("fps", STRING_FORMAT("FPS %d", FPS_UTIL.fps()));
    
    r.renderTextViews("main", "sprite", Color::RED);

    r.renderCurrentlyBoundFramebufferToScreen();
    
    renderAudio();
}

void DanteGameEngineState::populateFromEntityLayout(EntityLayoutDef& eld)
{
    EntityLayoutManagerLoader::loadEntityLayout(eld, _entityIDManager, false);
    world().populateFromEntityLayout(eld);
}

Entity* DanteGameEngineState::getPlayer(uint8_t playerID)
{
    Entity* ret = nullptr;
    
    std::string players;
    
    for (Entity* e : world().getPlayers())
    {
        players += StringUtil::toString(e->playerInfo()._playerID);
        players += ",";
        if (playerID == e->playerInfo()._playerID)
        {
            ret = e;
            break;
        }
    }
    
    return ret;
}

Entity* DanteGameEngineState::getControlledPlayer()
{
    return getPlayer(_inputProcessor.inputState().playerInputState(0)._playerID);
}

DanteGameInputProcessor& DanteGameEngineState::input()
{
    return _inputProcessor;
}

std::map<uint8_t, Entity::PlayerInfo>& DanteGameEngineState::players()
{
    return _players;
}

World& DanteGameEngineState::world()
{
    return *_world;
}

void DanteGameEngineState::joinServer(Engine* e)
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
                                             cb_dante_steam_getPlayerAddressHash,
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
                          cb_dante_client_onEntityRegistered,
                          cb_dante_client_onEntityDeregistered,
                          cb_dante_client_onPlayerWelcomed);
    
    if (NW_CLNT->connect() == false)
    {
        LOG("Unable to connect, exiting...");
        e->popState();
        return;
    }
}

void DanteGameEngineState::updateWithNetwork(Engine* e)
{
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
            EntityLayoutDef& eld = ENTITY_LAYOUT_MGR.entityLayoutDef(entityLayoutKey);
            ENGINE_STATE_GAME_DANTE.populateFromEntityLayout(eld);
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
            
            _numMovesToReprocess = numMovesProcessedLocally - world().getNumMovesProcessed();
            
            if (_numMovesToReprocess > 0)
            {
                if (ENGINE_CFG.networkLoggingEnabled())
                {
                    LOG("Rollback replaying last %d moves", _numMovesToReprocess);
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

void DanteGameEngineState::updateWorld(const Move& move)
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
    world().storeToCache();
}

void DanteGameEngineState::renderWithNetwork(Renderer& r)
{
    r.setTextVisible("rtt", true);
    r.setTextVisible("rollbackFrames", true);
    r.setTextVisible("inBps", true);
    r.setTextVisible("outBps", true);
    r.setTextVisible("player4Info", true);
    r.setTextVisible("player3Info", true);
    r.setTextVisible("player2Info", true);
    r.setTextVisible("player1Info", true);
    
    if (NW_CLNT)
    {
        r.setText("rtt", STRING_FORMAT("RTT %d ms", static_cast<int>(NW_CLNT->avgRoundTripTime())));
        r.setText("rollbackFrames", STRING_FORMAT("Rollback frames %d", _numMovesToReprocess));
        r.setText("inBps", STRING_FORMAT(" In %d Bps", static_cast<int>(NW_CLNT->bytesReceivedPerSecond())));
        r.setText("outBps", STRING_FORMAT("Out %d Bps", static_cast<int>(NW_CLNT->bytesSentPerSecond())));
    }
    
    Entity* player4 = getPlayer(4);
    if (player4 != nullptr)
    {
        if (NW_CLNT->isPlayerIDLocal(4) &&
            NW_CLNT->isPlayerIDLocal(1))
        {
            r.setText("player4Info", STRING_FORMAT("4|%s [.] to drop", player4->playerInfo()._playerName.c_str()));
        }
        else
        {
            r.setText("player4Info", STRING_FORMAT("4|%s", player4->playerInfo()._playerName.c_str()));
        }
    }
    else
    {
        r.setText("player4Info", "4|Maybe someone will join...?");
    }
    
    Entity* player3 = getPlayer(3);
    if (player3 != nullptr)
    {
        if (NW_CLNT->isPlayerIDLocal(3) &&
            NW_CLNT->isPlayerIDLocal(1))
        {
            r.setText("player3Info", STRING_FORMAT("3|%s [.] to drop", player3->playerInfo()._playerName.c_str()));
        }
        else
        {
            r.setText("player3Info", STRING_FORMAT("3|%s", player3->playerInfo()._playerName.c_str()));
        }
    }
    else
    {
        r.setText("player3Info", "3|Maybe someone will join...?");
    }
    
    Entity* player2 = getPlayer(2);
    if (player2 != nullptr)
    {
        if (NW_CLNT->isPlayerIDLocal(2) &&
            NW_CLNT->isPlayerIDLocal(1))
        {
            r.setText("player2Info", STRING_FORMAT("2|%s [.] to drop", player2->playerInfo()._playerName.c_str()));
        }
        else
        {
            r.setText("player2Info", STRING_FORMAT("2|%s", player2->playerInfo()._playerName.c_str()));
        }
    }
    else
    {
        r.setText("player2Info", "2|Connect gamepad or use arrow keys...");
    }
    
    Entity* player1 = getPlayer(1);
    if (player1 != nullptr)
    {
        r.setText("player1Info", STRING_FORMAT("1|%s", player1->playerInfo()._playerName.c_str()));
    }
    else
    {
        r.setText("player1Info", "1|Joining...");
    }
}

void DanteGameEngineState::renderAudio()
{
    if (AUDIO_ENGINE.isPaused() ||
        world().getNumMovesProcessed() == 0)
    {
        return;
    }
    
    uint32_t currentMoveIndex = world().getNumMovesProcessed() - 1;
    SoundFrameState& sfs = soundFrameStateAtMoveIndex(currentMoveIndex);
    if (sfs._frame < currentMoveIndex)
    {
        sfs._frame = currentMoveIndex;
        sfs._entitySoundStates.clear();
    }
    
    for (Entity* e : world().getPlayers())
    {
        playSoundForEntityIfNecessary(*e, currentMoveIndex);
    }
    
    for (Entity* e : world().getDynamicEntities())
    {
        playSoundForEntityIfNecessary(*e, currentMoveIndex);
    }
}

DanteGameEngineState::SoundFrameState& DanteGameEngineState::soundFrameStateAtMoveIndex(uint32_t moveIndex)
{
    uint32_t index = moveIndex % 360;
    
    return _soundFrameStates[index];
}

void DanteGameEngineState::playSoundForEntityIfNecessary(Entity& e, uint32_t moveIndex)
{
    // TODO, support more than 1 sound per entity per frame
    
    if (e.isExiled())
    {
        return;
    }
    
    std::string textureMapping = e.renderController()->getTextureMapping();
    std::vector<SoundMapping>* soundMappings = e.renderController()->getSoundMapping();
    if (soundMappings == nullptr)
    {
        return;
    }
    
    uint16_t soundStateTime = e.stateTime();
    Animation* animation = ASSETS_MGR.animation(textureMapping);
    if (animation != nullptr &&
        animation->isLooping() &&
        // TODO, this is just a crude hack to make sure
        // we don't hear the drill stomp sound effect more than once
        animation->cycleTimeBeforeFirstLoopingFrame() == 0)
    {
        soundStateTime = e.stateTime() % animation->cycleTime();
    }
    
    SoundMapping* soundMapping = nullptr;
    for (int i = 0; i < soundMappings->size(); ++i)
    {
        SoundMapping& sm = soundMappings->at(i);
        if (soundStateTime >= sm._stateTime)
        {
            soundMapping = &sm;
        }
    }
    
    if (soundMapping == nullptr)
    {
        return;
    }
    
    std::string soundID = soundMapping->_soundID;
    uint16_t stateTimeToCheck = soundStateTime - soundMapping->_stateTime;
    
    uint32_t moveIndexToCheck = 0;
    uint32_t numFramesToSkip = 0;
    if (moveIndex >= stateTimeToCheck)
    {
        moveIndexToCheck = moveIndex - stateTimeToCheck;
        numFramesToSkip = soundStateTime - soundMapping->_stateTime;
    }
    
    std::string soundStateMapping = STRING_FORMAT("%d+%s+%d+%d", e.getID(), soundID.c_str(), e.state()._state, soundMapping->_stateTime);
    
    SoundFrameState& sfs = soundFrameStateAtMoveIndex(moveIndexToCheck);
    
    std::map<std::string, uint32_t>* entitySoundState = nullptr;
    
    const auto& entitySoundStateQuery = sfs._entitySoundStates.find(e.getID());
    if (entitySoundStateQuery == sfs._entitySoundStates.end())
    {
        sfs._entitySoundStates.emplace(e.getID(), std::map<std::string, uint32_t>());
        sfs._frame = moveIndexToCheck;
        entitySoundState = &sfs._entitySoundStates[e.getID()];
    }
    else
    {
        entitySoundState = &entitySoundStateQuery->second;
    }
    
    const auto& soundHandleQuery = entitySoundState->find(soundStateMapping);
    if (soundHandleQuery == entitySoundState->end())
    {
        if (ENGINE_CFG.logOpenAL())
        {
            LOG("Playing soundStateMapping: %s", soundStateMapping.c_str());
        }
        uint32_t soundHandle = AUDIO_ENGINE.playSound(soundID, numFramesToSkip);
        
        if (ENGINE_CFG.logOpenAL())
        {
            LOG("Played sound %d seeked ahead %d frames", soundHandle, numFramesToSkip);
        }
        
        entitySoundState->emplace(soundStateMapping, soundHandle);
    }
}

DanteGameEngineState::DanteGameEngineState() : EngineState("json/game/Config.json"),
_entityIDManager(),
_timeTracker(),
_world(nullptr),
_inputProcessor(),
_numMovesToReprocess(0),
_scale(1.0)
{
    // Empty
}
