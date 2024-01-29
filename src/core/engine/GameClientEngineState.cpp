//
//  GameClientEngineState.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/27/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
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

GameInputProcessorState GameInputProcessor::update()
{
    uint16_t& inputStateP1 = _inputState.playerInputState(0)._inputState;
    uint16_t& inputStateP2 = _inputState.playerInputState(1)._inputState;
    
#if IS_MOBILE
    for (CursorEvent* e : INPUT_MGR.getCursorEvents())
    {
        uint16_t& inputState = inputStateP1;
        
        Vector2& pos = INPUT_MGR.convert(e);
        
        SET_BIT(inputState, ISF_MOVING_LEFT, e->isPressed() && pos._x < 38);
        SET_BIT(inputState, ISF_MOVING_RIGHT, e->isPressed() && pos._x > 76);
        
        SET_BIT(inputState, ISF_JUMPING, e->isDown() && pos._y > 32);
        
        if (e->isUp() && pos._x > 90 && pos._y < 8)
        {
            _state = GIMS_EXIT;
        }
    }
#endif
    
    for (GamepadEvent* e : INPUT_MGR.getGamepadEvents())
    {
        if (_state == GIMS_EXIT)
        {
            break;
        }
        
        uint16_t& inputState = e->_index == 0 ? inputStateP1 : inputStateP2;
        
        switch (e->_button)
        {
            case GPEB_BUTTON_SELECT:
            case GPEB_BUTTON_SNES_SELECT:
                _state = e->isDown() ? GIMS_EXIT : GIMS_DEFAULT;
                break;
            case GPEB_BUTTON_A:
                SET_BIT(inputState, ISF_EXECUTING_ATTACK, e->isPressed());
                break;
            case GPEB_BUTTON_B:
                SET_BIT(inputState, ISF_JUMPING, e->isPressed());
                break;
            case GPEB_BUTTON_Y:
            case GPEB_BUMPER_LEFT:
                // Weird that Y button on my SNES controller is coming through as GPEB_BUMPER_LEFT
                SET_BIT(inputState, ISF_EXECUTING_ABILITY, e->isPressed());
                break;
            case GPEB_BUTTON_X:
                SET_BIT(inputState, ISF_TRIGGERING_SPECIAL, e->isPressed());
                break;
            case GPEB_D_PAD_LEFT:
            {
                SET_BIT(inputState, ISF_MOVING_LEFT, e->isPressed());
                break;
            }
            case GPEB_D_PAD_RIGHT:
            {
                SET_BIT(inputState, ISF_MOVING_RIGHT, e->isPressed());
                break;
            }
            case GPEB_STICK_LEFT:
            {
                SET_BIT(inputState, ISF_MOVING_LEFT, e->_x < 0);
                SET_BIT(inputState, ISF_MOVING_RIGHT, e->_x > 0);
                break;
            }
            case GPEB_UNKNOWN_6:
            {
                _state = e->isPressed() ? GIMS_ZOOM_IN : GIMS_DEFAULT;
                SET_BIT(inputState, ISF_WARMING_UP, e->isPressed());
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
        switch (e->_key)
        {
            case GOW_KEY_ESCAPE:
                _state = e->isDown() ? GIMS_EXIT : GIMS_DEFAULT;
                break;
            case GOW_KEY_P:
                if (e->isDown())
                {
                    _state = _state == GIMS_DISPLAY_PHYSICS ? GIMS_DEFAULT : GIMS_DISPLAY_PHYSICS;
                }
                break;
            case GOW_KEY_J:
                SET_BIT(inputStateP1, ISF_JUMPING, e->isPressed());
                break;
            case GOW_KEY_K:
                SET_BIT(inputStateP1, ISF_EXECUTING_ATTACK, e->isPressed());
                break;
            case GOW_KEY_H:
                SET_BIT(inputStateP1, ISF_EXECUTING_ABILITY, e->isPressed());
                break;
            case GOW_KEY_U:
                SET_BIT(inputStateP1, ISF_TRIGGERING_SPECIAL, e->isPressed());
                break;
            case GOW_KEY_T:
                SET_BIT(inputStateP1, ISF_WARMING_UP, e->isPressed());
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
            case GOW_KEY_A:
                SET_BIT(inputStateP1, ISF_MOVING_LEFT, e->isPressed());
                break;
            case GOW_KEY_D:
                SET_BIT(inputStateP1, ISF_MOVING_RIGHT, e->isPressed());
                break;
            case GOW_KEY_ARROW_UP:
                SET_BIT(inputStateP2, ISF_JUMPING, e->isPressed());
                break;
            case GOW_KEY_ARROW_LEFT:
                SET_BIT(inputStateP2, ISF_MOVING_LEFT, e->isPressed());
                break;
            case GOW_KEY_ARROW_RIGHT:
                SET_BIT(inputStateP2, ISF_MOVING_RIGHT, e->isPressed());
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

void cb_client_onEntityRegistered(Entity* e)
{
    ENGINE_STATE_GAME_CLNT.world().addNetworkEntity(e);
}

void cb_client_onEntityDeregistered(Entity* e)
{
    ENGINE_STATE_GAME_CLNT.world().removeNetworkEntity(e);
}

void cb_client_onPlayerWelcomed(uint8_t playerID)
{
    ENGINE_STATE_GAME_CLNT.input().inputState().activateNextPlayer(playerID);
}

void GameClientEngineState::onEnter(Engine* e)
{
    // Empty
}

void GameClientEngineState::onAssetsLoaded(Engine* e)
{
    if (_args.getBool(ARG_OFFLINE_MODE, false) == false)
    {
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
        
        // TODO, add Steam implementation
        std::string serverAddress = STRING_FORMAT("%s:%d", serverIPAddress.c_str(), ENGINE_CFG.serverPort());
        std::string username = _args.getString(ARG_USERNAME);
        SocketClientHelper* clientHelper = new SocketClientHelper(_timeTracker, serverAddress, username, port, NetworkClient::sProcessPacket);
        
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
    
    if (!ENGINE_CFG.musicDisabled())
    {
        AUDIO_ENGINE.playSound("music_game", 0, 0.7f, true);
    }
}

void GameClientEngineState::onExit(Engine* e)
{
    if (_args.getBool(ARG_OFFLINE_MODE, false) == false)
    {
        NetworkClient::destroy();
    }
    
    _timeTracker.reset();
    world().reset();
    world().removeAllNetworkEntities();
    
    _inputProcessor.reset();
    _scale = 1.0f;
}

void GameClientEngineState::onUpdate(Engine* e)
{
    _timeTracker.onFrame();
    
    GameInputProcessorState gims = _inputProcessor.update();
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
    
    if (_args.getBool(ARG_OFFLINE_MODE, false))
    {
        updateOffline(e);
    }
    else
    {
        updateWithNetwork(e);
    }
}

void GameClientEngineState::onRender(Renderer& r, double extrapolation)
{
    const float baseRight = r.matrix()._base._right;
    const float baseTop = r.matrix()._base._top;
    float rightEdge = _world->rightEdge();
    float topEdge = _world->topEdge();
    float maxWidthScale = rightEdge / baseRight;
    float maxHeightScale = topEdge / baseTop;
    float maxScale = CLAMP(GOW_MIN(maxWidthScale, maxHeightScale), 1.0f, 32.0f);
    _scale = CLAMP(_scale, 0.25f, maxScale);
    
    if (ENGINE_CFG.extrapolatePhysics())
    {
        _world->extrapolatePhysics(extrapolation);
    }
    
    r.bindFramebuffer();
    
    Entity* controlledPlayer = getControlledPlayer();
    if (controlledPlayer != nullptr)
    {
        r.updateMatrixCenteredOnEntity(controlledPlayer, static_cast<float>(_world->rightEdge()), static_cast<float>(_world->topEdge()), _scale);
    }
    
    r.renderParallaxLayers(world().getLayers(), "background_upper");
    r.renderParallaxLayers(world().getLayers(), "background_mid");
    r.renderParallaxLayers(world().getLayers(), "background_lower");
    
    // TODO FIXME
    // Obviously, this is prone to error
    for (Entity* e : world().getStaticEntities())
    {
        r.spriteBatcherBegin();
        r.spriteBatcherAddEntity(e);
        std::string name = e->entityDef()._keyName;
        std::string texture = name == "P001" || name == "P002" || name == "P003"|| name == "P004" ? "platforming_1" : "ground";
        r.spriteBatcherEnd(texture);
    }
    
    if (_args.getBool(ARG_OFFLINE_MODE, false))
    {
        renderOffline(r);
    }
    else
    {
        renderWithNetwork(r);
    }
    
    if (_inputProcessor.state() == GIMS_DISPLAY_PHYSICS)
    {
        r.renderNosPhysics(static_cast<NosPhysicsWorld*>(_world));
    }
    
    r.setText("fps", STRING_FORMAT("FPS %d", FPS_UTIL.fps()));
    
    r.renderTextViews("main", "texture", Color::RED);

    r.renderCurrentlyBoundFramebufferToScreen();
    
    if (ENGINE_CFG.extrapolatePhysics())
    {
        _world->endExtrapolatedPhysics();
    }
    
    renderAudio();
}

void GameClientEngineState::populateFromEntityLayout(EntityLayoutDef& eld)
{
    EntityLayoutManagerLoader::loadEntityLayout(eld, _entityIDManager, false);
    world().populateFromEntityLayout(eld);
}

Entity* GameClientEngineState::getPlayer(uint8_t playerID)
{
    Entity* ret = nullptr;
    
    for (Entity* e : world().getPlayers())
    {
        if (playerID == e->networkDataField("playerID").valueUInt8())
        {
            ret = e;
            break;
        }
    }
    
    return ret;
}

Entity* GameClientEngineState::getControlledPlayer()
{
    return getPlayer(_inputProcessor.inputState().playerInputState(0)._playerID);
}

GameInputProcessor& GameClientEngineState::input()
{
    return _inputProcessor;
}

World& GameClientEngineState::world()
{
    return *_world;
}

void GameClientEngineState::updateWithNetwork(Engine* e)
{
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
            ENGINE_STATE_GAME_CLNT.populateFromEntityLayout(eld);
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
            assert (getPlayer(1) != nullptr);
            
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

void GameClientEngineState::updateOffline(Engine* e)
{
    if (getPlayer(1) == nullptr)
    {
        uint32_t spawnX = 64;
        uint32_t spawnY = 128;

        uint32_t key = 'JON0';
        uint32_t networkID = _entityIDManager.getNextPlayerEntityID();
        EntityInstanceDef eid(networkID, key, spawnX, spawnY, true);
        Entity* e = ENTITY_MGR.createEntity(eid);
        e->networkDataField("playerID").setValueUInt8(1);
        world().addNetworkEntity(e);
        
        world().resetNumMovesProcessed();
        input().setNumMovesProcessed(world().getNumMovesProcessed());
        
        input().inputState().activateNextPlayer(1);
        
        EntityLayoutDef& eld = ENTITY_LAYOUT_MGR.entityLayoutDef('L001');
        ENGINE_STATE_GAME_CLNT.populateFromEntityLayout(eld);
    }
    
    MoveList& ml = _inputProcessor.moveList();
    
    _inputProcessor.sampleInputAsNewMove(_timeTracker);
    Move* move = ml.getMoveWithMoveIndex(world().getNumMovesProcessed());
    updateWorld(*move);
    
    world().clearCache(world().getNumMovesProcessed());
    input().removeProcessedMovesWithIndexLessThan(world().getNumMovesProcessed());
}

void GameClientEngineState::updateWorld(const Move& move)
{
    world().beginFrame();
    for (Entity* e : world().getPlayers())
    {
        uint16_t inputState = e->lastProcessedInputState();
        uint8_t playerID = e->networkDataField("playerID").valueUInt8();
        
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
    if (_args.getBool(ARG_OFFLINE_MODE, false))
    {
        std::vector<Entity*> toDelete = world().update();
        for (Entity* e : toDelete)
        {
            world().removeNetworkEntity(e);
        }
    }
    else
    {
        world().update();
    }
    world().storeToCache();
}

void GameClientEngineState::renderWithNetwork(Renderer& r)
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
    
    r.setText("rtt", STRING_FORMAT("RTT %d ms", static_cast<int>(NW_CLNT->avgRoundTripTime())));
    r.setText("rollbackFrames", STRING_FORMAT("Rollback frames %d", _numMovesToReprocess));
    r.setText("inBps", STRING_FORMAT(" In %d Bps", static_cast<int>(NW_CLNT->bytesReceivedPerSecond())));
    r.setText("outBps", STRING_FORMAT("Out %d Bps", static_cast<int>(NW_CLNT->bytesSentPerSecond())));
    
    bool isReleasingShockwavePlayer4 = false;
    bool isVampirePlayer4 = false;
    uint16_t shockwaveStateTimePlayer4 = 0;
    Entity* player4 = getPlayer(4);
    if (player4 != nullptr)
    {
        if (NW_CLNT->isPlayerIDLocal(4) &&
            NW_CLNT->isPlayerIDLocal(1))
        {
            r.setText("player4Info", STRING_FORMAT("4|%s [.] to drop", player4->networkDataField("username").valueString().c_str()));
        }
        else
        {
            r.setText("player4Info", STRING_FORMAT("4|%s", player4->networkDataField("username").valueString().c_str()));
        }
        
        if (!player4->isExiled())
        {
            r.spriteBatcherBegin();
            r.spriteBatcherAddEntity(player4);
            std::string textureRegionKey = player4->renderController()->getTextureMapping();
            std::string textureForRegionKey = ASSETS_MGR.textureForRegionKey(textureRegionKey);
            r.spriteBatcherEnd(textureForRegionKey, "main", "texture", "main", Color::BLUE);
            
            JonController* jon = player4->controller<JonController>();
            isReleasingShockwavePlayer4 = jon->isReleasingShockwave();
            isVampirePlayer4 = jon->isVampire();
            shockwaveStateTimePlayer4 = jon->shockwaveStateTime();
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
            r.setText("player3Info", STRING_FORMAT("3|%s [.] to drop", player3->networkDataField("username").valueString().c_str()));
        }
        else
        {
            r.setText("player3Info", STRING_FORMAT("3|%s", player3->networkDataField("username").valueString().c_str()));
        }
        
        if (!player3->isExiled())
        {
            r.spriteBatcherBegin();
            r.spriteBatcherAddEntity(player3);
            std::string textureRegionKey = player3->renderController()->getTextureMapping();
            std::string textureForRegionKey = ASSETS_MGR.textureForRegionKey(textureRegionKey);
            r.spriteBatcherEnd(textureForRegionKey, "main", "texture", "main", Color::GREEN);
            
            JonController* jon = player3->controller<JonController>();
            isReleasingShockwavePlayer3 = jon->isReleasingShockwave();
            isVampirePlayer3 = jon->isVampire();
            shockwaveStateTimePlayer3 = jon->shockwaveStateTime();
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
            r.setText("player2Info", STRING_FORMAT("2|%s [.] to drop", player2->networkDataField("username").valueString().c_str()));
        }
        else
        {
            r.setText("player2Info", STRING_FORMAT("2|%s", player2->networkDataField("username").valueString().c_str()));
        }
        
        if (!player2->isExiled())
        {
            r.spriteBatcherBegin();
            r.spriteBatcherAddEntity(player2);
            std::string textureRegionKey = player2->renderController()->getTextureMapping();
            std::string textureForRegionKey = ASSETS_MGR.textureForRegionKey(textureRegionKey);
            r.spriteBatcherEnd(textureForRegionKey, "main", "texture", "main", Color::RED);
            
            JonController* jon = player2->controller<JonController>();
            isReleasingShockwavePlayer2 = jon->isReleasingShockwave();
            isVampirePlayer2 = jon->isVampire();
            shockwaveStateTimePlayer2 = jon->shockwaveStateTime();
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
        r.setText("player1Info", STRING_FORMAT("1|%s", player1->networkDataField("username").valueString().c_str()));
        
        if (!player1->isExiled())
        {
            r.spriteBatcherBegin();
            r.spriteBatcherAddEntity(player1);
            std::string textureRegionKey = player1->renderController()->getTextureMapping();
            std::string textureForRegionKey = ASSETS_MGR.textureForRegionKey(textureRegionKey);
            r.spriteBatcherEnd(textureForRegionKey);
            
            JonController* jon = player1->controller<JonController>();
            isReleasingShockwavePlayer1 = jon->isReleasingShockwave();
            isVampirePlayer1 = jon->isVampire();
            shockwaveStateTimePlayer1 = jon->shockwaveStateTime();
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

void GameClientEngineState::renderOffline(Renderer& r)
{
    static float frameRate = static_cast<float>(ENGINE_CFG.frameRate());
    
    r.setTextVisible("rtt", false);
    r.setTextVisible("rollbackFrames", false);
    r.setTextVisible("inBps", false);
    r.setTextVisible("outBps", false);
    r.setTextVisible("player4Info", false);
    r.setTextVisible("player3Info", false);
    r.setTextVisible("player2Info", false);
    r.setTextVisible("player1Info", false);
    
    bool isReleasingShockwavePlayer1 = false;
    bool isVampirePlayer1 = false;
    uint16_t shockwaveStateTimePlayer1 = 0;
    Entity* player1 = getPlayer(1);
    if (player1 != nullptr && !player1->isExiled())
    {
        r.spriteBatcherBegin();
        r.spriteBatcherAddEntity(player1);
        std::string textureRegionKey = player1->renderController()->getTextureMapping();
        std::string textureForRegionKey = ASSETS_MGR.textureForRegionKey(textureRegionKey);
        r.spriteBatcherEnd(textureForRegionKey);
        
        JonController* jon = player1->controller<JonController>();
        isReleasingShockwavePlayer1 = jon->isReleasingShockwave();
        isVampirePlayer1 = jon->isVampire();
        shockwaveStateTimePlayer1 = jon->shockwaveStateTime();
    }
    
    r.bindFramebuffer("player1");
    if (isReleasingShockwavePlayer1)
    {
        r.renderFramebufferWithShockwave("main", player1->position()._x, player1->position()._y, shockwaveStateTimePlayer1 * frameRate, isVampirePlayer1);
    }
    else
    {
        r.renderFramebuffer("main", isVampirePlayer1 ? "framebufferWithTint" : "framebuffer");
    }
}

void GameClientEngineState::renderAudio()
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

GameClientEngineState::SoundFrameState& GameClientEngineState::soundFrameStateAtMoveIndex(uint32_t moveIndex)
{
    uint32_t index = moveIndex % 360;
    
    return _soundFrameStates[index];
}

void GameClientEngineState::playSoundForEntityIfNecessary(Entity& e, uint32_t moveIndex)
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

GameClientEngineState::GameClientEngineState() : EngineState("data/json/GameEngineState/Config.json"),
_entityIDManager(),
_timeTracker(),
_world(new NosPhysicsWorld()),
_inputProcessor(),
_numMovesToReprocess(0),
_scale(1.0)
{
    // Empty
}
