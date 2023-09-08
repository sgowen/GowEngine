//
//  GameClientEngineState.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/27/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "EngineState.hpp"

#include "core/input/InputState.hpp"
#include "core/network/MoveList.hpp"
#include "core/entity/EntityIDManager.hpp"

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
    
    GameInputProcessorState update();
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

class GameClientEngineState : public EngineState
{    
public:    
    static GameClientEngineState& getInstance()
    {
        static GameClientEngineState ret = GameClientEngineState();
        return ret;
    }
    
    virtual void onEnter(Engine* e);
    virtual void onAssetsLoaded(Engine* e);
    virtual void onExit(Engine* e);
    virtual void onUpdate(Engine* e);
    virtual void onRender(Renderer& r, double extrapolation);
    
    void populateFromEntityLayout(EntityLayoutDef& eld);
    Entity* getPlayer(uint8_t playerID);
    Entity* getControlledPlayer();
    GameInputProcessor& input();
    World& world();
    
private:
    EntityIDManager _entityIDManager;
    TimeTracker _timeTracker;
    World* _world;
    GameInputProcessor _inputProcessor;
    int _numMovesToReprocess;
    float _scale;
    
    class SoundFrameState {
    public:
        std::map<uint32_t, std::map<std::string, uint32_t> > _entitySoundStates;
        uint32_t _frame;
        
        SoundFrameState() :
        _frame(0)
        {
            // Empty
        }
    };
    SoundFrameState _soundFrameStates[360];
    
    void updateWithNetwork(Engine* e);
    void updateOffline(Engine* e);
    void updateWorld(const Move& move);
    
    void renderWithNetwork(Renderer& r);
    void renderOffline(Renderer& r);
    
    void renderAudio();
    SoundFrameState& soundFrameStateAtMoveIndex(uint32_t moveIndex);
    void playSoundForEntityIfNecessary(Entity& e, uint32_t moveIndex);
    
    void spawnPlayerAndResetWorld();
    
    GameClientEngineState();
    virtual ~GameClientEngineState() {}
    GameClientEngineState(const GameClientEngineState&);
    GameClientEngineState& operator=(const GameClientEngineState&);
};
