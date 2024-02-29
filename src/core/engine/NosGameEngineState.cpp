//
//  NosGameEngineState.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/27/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

NosGameInputProcessor::NosGameInputProcessor() :
_inputState(),
_state(NGIMS_DEFAULT),
_numMovesProcessed(0)
{
    reset();
}

NosGameInputProcessor::~NosGameInputProcessor()
{
    // Empty
}

NosGameInputProcessorState NosGameInputProcessor::update()
{
    uint16_t& inputStateP1 = _inputState.playerInputState(0)._inputState;
    uint16_t& inputStateP2 = _inputState.playerInputState(1)._inputState;
    
#if IS_MOBILE
    for (CursorEvent* e : INPUT_MGR.getCursorEvents())
    {
        uint16_t& inputState = inputStateP1;
        
        Vector2& pos = INPUT_MGR.convert(e);
        
        SET_BIT(inputState, JISF_MOVING_LEFT, e->isPressed() && pos._x < 38);
        SET_BIT(inputState, JISF_MOVING_RIGHT, e->isPressed() && pos._x > 76);
        
        SET_BIT(inputState, JISF_JUMPING, e->isDown() && pos._y > 32);
        
        if (e->isUp() && pos._x > 90 && pos._y < 8)
        {
            _state = NGIMS_EXIT;
        }
    }
#endif
    
    for (GamepadEvent* e : INPUT_MGR.getGamepadEvents())
    {
        if (_state == NGIMS_EXIT)
        {
            break;
        }
        
        uint16_t& inputState = e->_index == 0 ? inputStateP1 : inputStateP2;
        
        switch (e->_button)
        {
            case GPEB_BUTTON_SELECT:
            case GPEB_BUTTON_SNES_SELECT:
                _state = e->isDown() ? NGIMS_EXIT : NGIMS_DEFAULT;
                break;
            case GPEB_BUTTON_A:
                SET_BIT(inputState, JISF_EXECUTING_ATTACK, e->isPressed());
                break;
            case GPEB_BUTTON_B:
                SET_BIT(inputState, JISF_JUMPING, e->isPressed());
                break;
            case GPEB_BUTTON_Y:
            case GPEB_BUMPER_LEFT:
                // Weird that Y button on my SNES controller is coming through as GPEB_BUMPER_LEFT
                SET_BIT(inputState, JISF_EXECUTING_ABILITY, e->isPressed());
                break;
            case GPEB_BUTTON_X:
                SET_BIT(inputState, JISF_TRIGGERING_SPECIAL, e->isPressed());
                break;
            case GPEB_D_PAD_LEFT:
            {
                SET_BIT(inputState, JISF_MOVING_LEFT, e->isPressed());
                break;
            }
            case GPEB_D_PAD_RIGHT:
            {
                SET_BIT(inputState, JISF_MOVING_RIGHT, e->isPressed());
                break;
            }
            case GPEB_STICK_LEFT:
            {
                SET_BIT(inputState, JISF_MOVING_LEFT, e->_x < 0);
                SET_BIT(inputState, JISF_MOVING_RIGHT, e->_x > 0);
                break;
            }
            case GPEB_UNKNOWN_6:
            {
                _state = e->isPressed() ? NGIMS_ZOOM_IN : NGIMS_DEFAULT;
                SET_BIT(inputState, JISF_WARMING_UP, e->isPressed());
                break;
            }
            case GPEB_BUMPER_RIGHT:
            case GPEB_UNKNOWN_7:
                if (_state == NGIMS_ZOOM_IN)
                {
                    _state = NGIMS_ZOOM_RESET;
                }
                else
                {
                    _state = e->isPressed() ? NGIMS_ZOOM_OUT : NGIMS_DEFAULT;
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
                _state = e->isUp() ? NGIMS_EXIT : NGIMS_DEFAULT;
                break;
            case GOW_KEY_P:
                if (e->isDown())
                {
                    _state = _state == NGIMS_DISPLAY_PHYSICS ? NGIMS_DEFAULT : NGIMS_DISPLAY_PHYSICS;
                }
                break;
            case GOW_KEY_J:
                SET_BIT(inputStateP1, JISF_JUMPING, e->isPressed());
                break;
            case GOW_KEY_K:
                SET_BIT(inputStateP1, JISF_EXECUTING_ATTACK, e->isPressed());
                break;
            case GOW_KEY_H:
                SET_BIT(inputStateP1, JISF_EXECUTING_ABILITY, e->isPressed());
                break;
            case GOW_KEY_U:
                SET_BIT(inputStateP1, JISF_TRIGGERING_SPECIAL, e->isPressed());
                break;
            case GOW_KEY_T:
                SET_BIT(inputStateP1, JISF_WARMING_UP, e->isPressed());
                break;
            case GOW_KEY_I:
                _state = e->isPressed() ? NGIMS_ZOOM_IN : NGIMS_DEFAULT;
                break;
            case GOW_KEY_O:
                if (_state == NGIMS_ZOOM_IN)
                {
                    _state = NGIMS_ZOOM_RESET;
                }
                else
                {
                    _state = e->isPressed() ? NGIMS_ZOOM_OUT : NGIMS_DEFAULT;
                }
                break;
            case GOW_KEY_A:
                SET_BIT(inputStateP1, JISF_MOVING_LEFT, e->isPressed());
                break;
            case GOW_KEY_D:
                SET_BIT(inputStateP1, JISF_MOVING_RIGHT, e->isPressed());
                break;
            case GOW_KEY_ARROW_UP:
                SET_BIT(inputStateP2, JISF_JUMPING, e->isPressed());
                break;
            case GOW_KEY_ARROW_LEFT:
                SET_BIT(inputStateP2, JISF_MOVING_LEFT, e->isPressed());
                break;
            case GOW_KEY_ARROW_RIGHT:
                SET_BIT(inputStateP2, JISF_MOVING_RIGHT, e->isPressed());
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

NosGameInputProcessorState NosGameInputProcessor::state()
{
    return _state;
}

void NosGameInputProcessor::sampleInputAsNewMove(TimeTracker& tt)
{
    InputState* inputState = _poolInputState.obtain();
    _inputState.copyTo(inputState);
    
    _moveList.addMove(inputState, tt._time, _numMovesProcessed);
    ++_numMovesProcessed;
}

void NosGameInputProcessor::removeProcessedMovesWithIndexLessThan(uint32_t numMovesProcessed)
{
    _moveList.removeProcessedMovesWithIndexLessThan(numMovesProcessed, _poolInputState);
}

InputState& NosGameInputProcessor::inputState()
{
    return _inputState;
}

MoveList& NosGameInputProcessor::moveList()
{
    return _moveList;
}

void NosGameInputProcessor::reset()
{
    _moveList.removeAllMoves(_poolInputState);
    _inputState.reset();
    _numMovesProcessed = 0;
    
    _state = NGIMS_DEFAULT;
}

void NosGameInputProcessor::setNumMovesProcessed(uint32_t numMovesProcessed)
{
    _numMovesProcessed = numMovesProcessed;
}

void NosGameInputProcessor::drop2ndPlayer()
{
    InputState::PlayerInputState& pis = _inputState.playerInputState(1);
    pis._playerID = 0;
    NW_CLNT->requestToDropLocalPlayer(1);
}

uint64_t cb_nos_steam_getPlayerAddressHash(uint8_t inPlayerIndex)
{
    uint64_t ret = 0;
    
    std::map<uint8_t, Entity::PlayerInfo>& players = ENGINE_STATE_GAME_NOS.players();
    
    uint8_t playerID = inPlayerIndex + 1;
    
    auto q = players.find(playerID);
    if (q != players.end())
    {
        Entity::PlayerInfo& playerInfo = q->second;
        ret = playerInfo._playerAddressHash;
    }
    
    return ret;
}

void cb_nos_client_onEntityRegistered(Entity* e)
{
    ENGINE_STATE_GAME_NOS.world().addNetworkEntity(e);
    
    if (e->isPlayer())
    {
        ENGINE_STATE_GAME_NOS.players().insert({e->playerInfo()._playerID, e->playerInfo()});
    }
}

void cb_nos_client_onEntityDeregistered(Entity* e)
{
    ENGINE_STATE_GAME_NOS.world().removeNetworkEntity(e);
}

void cb_nos_client_onPlayerWelcomed(uint8_t playerID)
{
    ENGINE_STATE_GAME_NOS.input().inputState().activateNextPlayer(playerID);
}

void NosGameEngineState::onEnter(Engine* e)
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
    
    if (_args.getBool(ARG_IS_HOST, false) == true)
    {
        GameServer::create(_config);
        assert(GAME_SERVER != nullptr);
    }
}

void NosGameEngineState::onAssetsLoaded(Engine* e)
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

void NosGameEngineState::onExit(Engine* e)
{
    if (_args.getBool(ARG_IS_HOST, false) == true)
    {
        GameServer::destroy();
    }
    
    NetworkClient::destroy();
    
    _timeTracker.reset();
    delete _world;
    
    _inputProcessor.reset();
    _scale = 1.0f;
}

void NosGameEngineState::onUpdate(Engine* e)
{
    if (GAME_SERVER)
    {
        GAME_SERVER->update();
    }
    
    _timeTracker.onFrame();
    
    NosGameInputProcessorState gims = _inputProcessor.update();
    if (gims == NGIMS_EXIT)
    {
        e->popState();
        return;
    }
    
    static float zoomStep = 0.035398230088496f;
    if (gims == NGIMS_ZOOM_IN)
    {
        _scale -= zoomStep;
    }
    if (gims == NGIMS_ZOOM_OUT)
    {
        _scale += zoomStep;
    }
    if (gims == NGIMS_ZOOM_RESET)
    {
        _scale = 1.0f;
    }
    
    updateWithNetwork(e);
}

void NosGameEngineState::onRender(Renderer& r, double extrapolation)
{
    const float baseRight = r.matrix()._base._right;
    const float baseTop = r.matrix()._base._top;
    float rightEdge = _world->rightEdge();
    float topEdge = _world->topEdge();
    float maxWidthScale = rightEdge / baseRight;
    float maxHeightScale = topEdge / baseTop;
    float maxScale = CLAMP(GOW_MIN(maxWidthScale, maxHeightScale), 1.0f, 32.0f);
    _scale = CLAMP(_scale, 0.125f, maxScale);
    
    if (ENGINE_CFG.extrapolatePhysics())
    {
        _world->extrapolatePhysics(extrapolation);
    }
    
    r.bindFramebuffer();
    
    Entity* controlledPlayer = getControlledPlayer();
    if (controlledPlayer != nullptr)
    {
        r.updateMatrixCenteredOnEntity(controlledPlayer, _world->rightEdge(), _world->topEdge(), _scale);
    }
    
    r.renderNosParallaxLayers(world().getLayers(), "background_upper");
    r.renderNosParallaxLayers(world().getLayers(), "background_mid");
    r.renderNosParallaxLayers(world().getLayers(), "background_lower");
    
    std::vector<Entity*> platformingEntities;
    r.spriteBatcherBegin();
    for (Entity* e : world().getStaticEntities())
    {
        // TODO, this isn't the best code you know
        std::string name = e->entityDef()._keyName;
        bool isPlatformingEntity = name == "P001" || name == "P002" || name == "P003"|| name == "P004";
        if (isPlatformingEntity)
        {
            platformingEntities.push_back(e);
        }
        else
        {
            r.spriteBatcherAddEntity(e);
        }
    }
    r.spriteBatcherEnd("ground");
    
    r.spriteBatcherBegin();
    for (Entity* e : platformingEntities)
    {
        r.spriteBatcherAddEntity(e);
    }
    r.spriteBatcherEnd("platforming_1");
    
    renderWithNetwork(r);
    
    if (_inputProcessor.state() == NGIMS_DISPLAY_PHYSICS)
    {
        r.renderNosPhysics(static_cast<NosPhysicsWorld*>(_world));
    }
    
    r.setText("fps", STRING_FORMAT("FPS %d", FPS_UTIL.fps()));
    
    r.renderTextViews("main", "sprite", Color::RED);

    r.renderCurrentlyBoundFramebufferToScreen();
    
    if (ENGINE_CFG.extrapolatePhysics())
    {
        _world->endExtrapolatedPhysics();
    }
    
    renderAudio();
}

void NosGameEngineState::populateFromEntityLayout(EntityLayoutDef& eld)
{
    EntityLayoutManagerLoader::loadEntityLayout(eld, _entityIDManager, false);
    world().populateFromEntityLayout(eld);
}

Entity* NosGameEngineState::getPlayer(uint8_t playerID)
{
    Entity* ret = nullptr;
    
    for (Entity* e : world().getPlayers())
    {
        if (playerID == e->playerInfo()._playerID)
        {
            ret = e;
            break;
        }
    }
    
    return ret;
}

Entity* NosGameEngineState::getControlledPlayer()
{
    return getPlayer(_inputProcessor.inputState().playerInputState(0)._playerID);
}

NosGameInputProcessor& NosGameEngineState::input()
{
    return _inputProcessor;
}

std::map<uint8_t, Entity::PlayerInfo>& NosGameEngineState::players()
{
    return _players;
}

World& NosGameEngineState::world()
{
    return *_world;
}

void NosGameEngineState::joinServer(Engine* e)
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
                                             cb_nos_steam_getPlayerAddressHash,
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
                          cb_nos_client_onEntityRegistered,
                          cb_nos_client_onEntityDeregistered,
                          cb_nos_client_onPlayerWelcomed);
    
    if (NW_CLNT->connect() == false)
    {
        LOG("Unable to connect, exiting...");
        e->popState();
        return;
    }
}

void NosGameEngineState::updateWithNetwork(Engine* e)
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
        world().recallCache(NW_CLNT->getNumMovesProcessed());
        world().clearCache(world().getNumMovesProcessed());
        input().removeProcessedMovesWithIndexLessThan(world().getNumMovesProcessed());
        input().setNumMovesProcessed(world().getNumMovesProcessed());
        
        Entity* e = getPlayer(1);
        if (e != nullptr)
        {
            uint32_t entityLayoutKey = e->networkDataField("entityLayoutKey").valueUInt32();
            EntityLayoutDef& eld = ENTITY_LAYOUT_MGR.entityLayoutDef(entityLayoutKey);
            ENGINE_STATE_GAME_NOS.populateFromEntityLayout(eld);
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
        assert (getPlayer(1) != nullptr);
    }
}

void NosGameEngineState::updateWorld(const Move& move)
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
    
    static float frameRate = static_cast<float>(ENGINE_CFG.frameRate());
    world().stepPhysics(frameRate);
    world().update();
    world().storeToCache();
}

void NosGameEngineState::renderWithNetwork(Renderer& r)
{
    static float frameRate = static_cast<float>(ENGINE_CFG.frameRate());
    
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
    
    bool isReleasingShockwavePlayer4 = false;
    bool isVampirePlayer4 = false;
    uint16_t shockwaveStateTimePlayer4 = 0;
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
        
        if (!player4->isExiled())
        {
            r.spriteBatcherBegin();
            r.spriteBatcherAddEntity(player4);
            std::string textureRegionKey = player4->renderController()->getTextureMapping();
            std::string textureForRegionKey = ASSETS_MGR.textureForRegionKey(textureRegionKey);
            r.spriteBatcherEnd(textureForRegionKey, "main", "sprite", "main", Color::BLUE);
            
            JonController* player = player4->controller<JonController>();
            isReleasingShockwavePlayer4 = player->isReleasingShockwave();
            isVampirePlayer4 = player->isVampire();
            shockwaveStateTimePlayer4 = player->shockwaveStateTime();
        }
    }
    else
    {
        r.setText("player4Info", "4|Maybe someone will join...?");
    }
    
    r.bindFramebuffer("player4");
    if (isReleasingShockwavePlayer4)
    {
        r.renderFramebufferWithShockwave("main", player4->position()._x, player4->position()._y, shockwaveStateTimePlayer4 * frameRate, isVampirePlayer4);
    }
    else
    {
        r.renderFramebuffer("main", isVampirePlayer4 ? "framebufferWithTint" : "framebuffer");
    }
    
    bool isReleasingShockwavePlayer3 = false;
    bool isVampirePlayer3 = false;
    uint16_t shockwaveStateTimePlayer3 = 0;
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
        
        if (!player3->isExiled())
        {
            r.spriteBatcherBegin();
            r.spriteBatcherAddEntity(player3);
            std::string textureRegionKey = player3->renderController()->getTextureMapping();
            std::string textureForRegionKey = ASSETS_MGR.textureForRegionKey(textureRegionKey);
            r.spriteBatcherEnd(textureForRegionKey, "main", "sprite", "main", Color::GREEN);
            
            JonController* player = player3->controller<JonController>();
            isReleasingShockwavePlayer3 = player->isReleasingShockwave();
            isVampirePlayer3 = player->isVampire();
            shockwaveStateTimePlayer3 = player->shockwaveStateTime();
        }
    }
    else
    {
        r.setText("player3Info", "3|Maybe someone will join...?");
    }
    
    r.bindFramebuffer("player3");
    if (isReleasingShockwavePlayer3)
    {
        r.renderFramebufferWithShockwave("player4", player3->position()._x, player3->position()._y, shockwaveStateTimePlayer3 * frameRate, isVampirePlayer3);
    }
    else
    {
        r.renderFramebuffer("player4", isVampirePlayer3 ? "framebufferWithTint" : "framebuffer");
    }
    
    bool isReleasingShockwavePlayer2 = false;
    bool isVampirePlayer2 = false;
    uint16_t shockwaveStateTimePlayer2 = 0;
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
        
        if (!player2->isExiled())
        {
            r.spriteBatcherBegin();
            r.spriteBatcherAddEntity(player2);
            std::string textureRegionKey = player2->renderController()->getTextureMapping();
            std::string textureForRegionKey = ASSETS_MGR.textureForRegionKey(textureRegionKey);
            r.spriteBatcherEnd(textureForRegionKey, "main", "sprite", "main", Color::RED);
            
            JonController* player = player2->controller<JonController>();
            isReleasingShockwavePlayer2 = player->isReleasingShockwave();
            isVampirePlayer2 = player->isVampire();
            shockwaveStateTimePlayer2 = player->shockwaveStateTime();
        }
    }
    else
    {
        r.setText("player2Info", "2|Connect gamepad or use arrow keys...");
    }
    
    r.bindFramebuffer("player2");
    if (isReleasingShockwavePlayer2)
    {
        r.renderFramebufferWithShockwave("player3", player2->position()._x, player2->position()._y, shockwaveStateTimePlayer2 * frameRate, isVampirePlayer2);
    }
    else
    {
        r.renderFramebuffer("player3", isVampirePlayer2 ? "framebufferWithTint" : "framebuffer");
    }
    
    bool isReleasingShockwavePlayer1 = false;
    bool isVampirePlayer1 = false;
    uint16_t shockwaveStateTimePlayer1 = 0;
    Entity* player1 = getPlayer(1);
    if (player1 != nullptr)
    {
        r.setText("player1Info", STRING_FORMAT("1|%s", player1->playerInfo()._playerName.c_str()));
        
        if (!player1->isExiled())
        {
            r.spriteBatcherBegin();
            r.spriteBatcherAddEntity(player1);
            std::string textureRegionKey = player1->renderController()->getTextureMapping();
            std::string textureForRegionKey = ASSETS_MGR.textureForRegionKey(textureRegionKey);
            r.spriteBatcherEnd(textureForRegionKey);
            
            JonController* player = player1->controller<JonController>();
            isReleasingShockwavePlayer1 = player->isReleasingShockwave();
            isVampirePlayer1 = player->isVampire();
            shockwaveStateTimePlayer1 = player->shockwaveStateTime();
        }
    }
    else
    {
        r.setText("player1Info", "1|Joining...");
    }
    
    r.bindFramebuffer("player1");
    if (isReleasingShockwavePlayer1)
    {
        r.renderFramebufferWithShockwave("player2", player1->position()._x, player1->position()._y, shockwaveStateTimePlayer1 * frameRate, isVampirePlayer1);
    }
    else
    {
        r.renderFramebuffer("player2", isVampirePlayer1 ? "framebufferWithTint" : "framebuffer");
    }
}

void NosGameEngineState::renderAudio()
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
}

NosGameEngineState::SoundFrameState& NosGameEngineState::soundFrameStateAtMoveIndex(uint32_t moveIndex)
{
    uint32_t index = moveIndex % 360;
    
    return _soundFrameStates[index];
}

void NosGameEngineState::playSoundForEntityIfNecessary(Entity& e, uint32_t moveIndex)
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

NosGameEngineState::NosGameEngineState() : EngineState("json/game/Config.json"),
_entityIDManager(),
_timeTracker(),
_world(nullptr),
_inputProcessor(),
_numMovesToReprocess(0),
_scale(1.0)
{
    // Empty
}
