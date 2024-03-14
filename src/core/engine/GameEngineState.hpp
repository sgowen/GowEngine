//
//  GameEngineState.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/1/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

#include "EngineState.hpp"

#include "core/input/InputState.hpp"
#include "core/network/MoveList.hpp"
#include "core/entity/EntityIDManager.hpp"
#include "core/entity/Entity.hpp"

enum GameInputProcessorState
{
    GIMS_DEFAULT,
    GIMS_DISPLAY_PHYSICS,
    GIMS_EXIT,
    GIMS_ZOOM_IN,
    GIMS_ZOOM_OUT,
    GIMS_ZOOM_RESET
};

class GameInputProcessor
{
public:
    GameInputProcessor();
    ~GameInputProcessor();
    
    GameInputProcessorState update(World& world);
    GameInputProcessorState state();
    void sampleInputAsNewMove(TimeTracker& tt);
    void removeProcessedMovesWithIndexLessThan(uint32_t numMovesProcessed);
    InputState& inputState();
    MoveList& moveList();
    void reset();
    void setNumMovesProcessed(uint32_t numMovesProcessed);
    
private:
    MoveList _moveList;
    Pool<InputState> _poolInputState;
    InputState _inputState;
    GameInputProcessorState _state;
    uint32_t _numMovesProcessed;
    
    void drop2ndPlayer();
};

class World;

class GameEngineState : public EngineState
{    
public:    
    static GameEngineState& getInstance()
    {
        static GameEngineState ret = GameEngineState();
        return ret;
    }
    
    void populateFromEntityLayout(EntityLayout& eld);
    Entity* getPlayer(uint8_t playerID);
    Entity* getControlledPlayer();
    GameInputProcessor& input();
    std::map<uint8_t, Entity::PlayerInfo>& players();
    World& world();

protected:
    virtual void onEnter(Engine* e) override;
    virtual void onAssetsLoaded(Engine* e) override;
    virtual void onExit(Engine* e) override;
    virtual void onUpdate(Engine* e) override;
    virtual void onRender(Renderer& r) override;
    
private:
    EntityIDManager _entityIDManager;
    TimeTracker _timeTracker;
    std::map<uint8_t, Entity::PlayerInfo> _players;
    World* _world;
    GameInputProcessor _inputProcessor;
    int _numRollbackFrames;
    float _scale;
    
    void joinServer(Engine* e);
    void updateWorld(const Move& move);
    
    GameEngineState();
    virtual ~GameEngineState() {}
    GameEngineState(const GameEngineState&);
    GameEngineState& operator=(const GameEngineState&);
};
